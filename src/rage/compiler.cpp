#include "compiler.h"

#include <QMessageBox>

#include "../util/util.h"

Compiler::Compiler(Script &script)
{
    m_origScript = &script;
}

int Compiler::roundUp(int value, int round)
{
    int pad = round - value % round;
    return (value == 0 || pad == round) ? value : value + pad;
}

int Compiler::roundDown(int value, int round)
{
    return (int)(value / round) * round;
}

QByteArray Compiler::compile()
{
    copy();
    clean();

    std::vector<WriteSize*> writeOrder;

    writeOrder.push_back(new WriteSize("codePages",   ((m_header.codePagesSize - 1) * 0x4000) + roundUp((m_header.codeSize % 0x4000), 16)));
    writeOrder.push_back(new WriteSize("statics",     roundUp(m_header.staticsSize * 4, 16)));
    writeOrder.push_back(new WriteSize("natives",     roundUp(m_header.nativesSize * 4, 16)));
    writeOrder.push_back(new WriteSize("codePagePtr", roundUp(m_header.codePagesSize * 4, 16)));
    writeOrder.push_back(new WriteSize("pageMapPtr",  roundUp(m_header.codePagesSize * 4 + 16, 16)));

    findWriteOrder(&writeOrder);

    unsigned int totalSize = 0;

    for (auto size : writeOrder)
    {
        totalSize += size->m_size;
    }

    totalSize += 0x30; // account for header

    totalSize = roundUp(totalSize, 0x1000);

    m_result.fill(0x00, totalSize);

    std::vector<unsigned int> dataPageSizes = getDataPageSizes(totalSize);
    std::vector<unsigned int> writtenOnPage = { 0, 0, 0, 0 };
    unsigned int bytesWritten = 0;

    m_header.headerPos = findHeaderPos(dataPageSizes);

    unsigned int overflow = 0;

    for (unsigned int i = 0; i < dataPageSizes.size(); i++)
    {
        if (writtenOnPage[i] == dataPageSizes[i])
        {
            continue;
        }

        if (overflow > 0)
        {
            if (overflow >= dataPageSizes[i])
            {
                overflow -= dataPageSizes[i];
                writtenOnPage[i] = dataPageSizes[i];

                continue;
            }
            else
            {
                writtenOnPage[i] = overflow;
                overflow = 0;
            }
        }

        if (i == dataPageSizes.size() - 1)
        {
            bytesWritten += 0x30;
            writtenOnPage[i] += 0x30;
        }

        unsigned int sizeLeft;

        for (auto toWrite : writeOrder)
        {
            sizeLeft = dataPageSizes[i] - writtenOnPage[i];

            if (toWrite->m_name != "codePages" && !toWrite->m_written && sizeLeft >= toWrite->m_size)
            {
                if (toWrite->m_name == "statics")
                {
                    m_header.staticsPtr = bytesWritten;
                    bytesWritten += writeStatics();
                }
                else if (toWrite->m_name == "natives")
                {
                    m_header.nativesPtr = bytesWritten;
                    bytesWritten += writeNatives();
                }
                else if (toWrite->m_name == "codePagePtr")
                {
                    m_header.codePagesPtr = bytesWritten;
                    bytesWritten += writeCodePageMap();
                }
                else if (toWrite->m_name == "pageMapPtr")
                {
                    m_header.pageMapPtr = bytesWritten;
                    bytesWritten += writePageMap();
                }

                toWrite->m_written = true;
                writtenOnPage[i] += toWrite->m_size;
                sizeLeft -= toWrite->m_size;
            }
            else if (toWrite->m_name == "codePages" && !toWrite->m_written)
            {
                bytesWritten += writeCode();
                toWrite->m_written = true;

                if (toWrite->m_size > dataPageSizes[i])
                {
                    overflow += toWrite->m_size - dataPageSizes[i];
                }

                writtenOnPage[i] += std::min(toWrite->m_size, dataPageSizes[i]);
                sizeLeft -= toWrite->m_size;
            }
        }

        if (bytesWritten + sizeLeft <= totalSize)
        {
            memset(m_result.data() + bytesWritten, 0xCD, sizeLeft);
            bytesWritten += sizeLeft;
        }
    }

    for (auto toWrite : writeOrder)
    {
        delete toWrite;
    }

    writeOrder.clear();

    int finalPad = totalSize - bytesWritten;
    memset(m_result.data() + bytesWritten, 0xCD, finalPad);

    writeHeader();

    return m_result;
}

// copy known data from original script
void Compiler::copy()
{
    m_header.magic       = m_origScript->getScriptHeader().magic;
    m_header.paramCount  = m_origScript->getScriptHeader().paramCount;
    m_header.globalsVers = m_origScript->getScriptHeader().globalsVers;

    m_header.nativesSize = m_origScript->getNatives().size();
    m_header.staticsSize = m_origScript->getStatics().size();

    m_natives = m_origScript->getNatives();
    m_statics = m_origScript->getStatics();
}

void Compiler::clean()
{
    int codeSize = 0;

    for (auto op : m_origScript->getOpcodes())
    {
        if (op->getOp() == EOpcodes::_SPACER || op->getDeleted())
        {
            continue;
        }

        op->setLocation(codeSize);

        if (op->getOp() == EOpcodes::_SUB)
        {
            m_subs.append(std::dynamic_pointer_cast<Op_HSub>(op));
            continue;
        }
        else if (op->getOp() == EOpcodes::OP_ENTER)
        {
            m_funcs.append(std::dynamic_pointer_cast<Op_Enter>(op));
        }

        codeSize += op->getFullData().size();

        m_code.append(op);
    }

    m_header.codeSize = codeSize;
    m_header.codePagesSize = (codeSize / 0x4000) + 1;
}

int Compiler::findHeaderPos(std::vector<unsigned int> pageSizes)
{
    unsigned int lastPage = pageSizes[pageSizes.size() - 1];
    int pos = 0;

    for (unsigned int i = 0; i < pageSizes.size(); i++)
    {
        if (pageSizes[i] != lastPage)
        {
            pos += pageSizes[i];
        }
        else
        {
            break;
        }
    }

    return pos;
}

void Compiler::findWriteOrder(std::vector<WriteSize*> *order)
{
    int vecsize = order->size();

    for (int j = 0; j < vecsize - 1; ++j)
    {
        int max = j;

        for (int i = j + 1; i < vecsize; ++i)
        {
            if (order->at(max)->m_size < order->at(i)->m_size)
            {
                max = i;
            }
        }

        if (max != j)
        {
            std::swap(order->at(j), order->at(max));
        }
    }
}

std::vector<unsigned int> Compiler::getDataPageSizes(int codeSize)
{
    int rem = codeSize % 4096;

    if (rem != 0)
        codeSize += 4096 - rem;

    std::vector<unsigned int> pageSizes;

    unsigned int pageCounts[4] = { 0, 0, 0, 0x7FFFFFFF };

    unsigned int pageSize  = 0x80000; // largest possible page size
    unsigned int totalSize = 0;
    unsigned int sizeLeft  = codeSize;

    for (unsigned int i = 0; i < 4; i++)
    {
        for (unsigned int j = 0; j < pageCounts[i] && sizeLeft; j++)
        {
            while (pageSize > sizeLeft)
                pageSize >>= 1;

            assert(pageSize && "PageSize = 0");

            totalSize += pageSize;
            sizeLeft -= pageSize;

            pageSizes.push_back(pageSize);
        }

        pageSize >>= 1;
    }

    return pageSizes;
}

int Compiler::writeCode()
{
    unsigned int bytesWritten = 0;
    int offset;
    int curOp = 0;

    for (unsigned int page = 0; page < (unsigned int)m_header.codePagesSize; page++)
    {
        unsigned int pageSize = (page == (unsigned int)m_header.codePagesSize - 1) ? m_header.codeSize % 0x4000 : 0x4000;

        offset = 0;

        if (bytesWritten == (unsigned int)m_header.headerPos)
        {
            offset += 0x30;
        }

        bytesWritten += offset;

        m_codePageOffsets.push_back(bytesWritten);

        // clear current page
        memset(m_result.data() + bytesWritten, 0, pageSize);

        while (bytesWritten < pageSize + m_codePageOffsets[page] && curOp < m_code.size())
        {
            auto op = m_code.at(curOp);

            unsigned int opSize = op->getFullData().size();

            if (bytesWritten + opSize > pageSize + m_codePageOffsets[page])
            {
                bytesWritten = roundUp(bytesWritten, 0x4000);
                break;
            }

            memcpy(m_result.data() + bytesWritten, op->getFullData().data(), opSize);

            bytesWritten += opSize;
            curOp++;
        }
    }

    int pad = roundUp(bytesWritten, 16) - bytesWritten;

    memset(m_result.data() + bytesWritten, 0xCD, pad);

    bytesWritten += pad;
    bytesWritten -= offset;

    // fix jump locations
//    for (auto sub : m_subs)
//    {
//        for (auto jump : sub->getReferences())
//        {
//            QDataStream stream(&jump->getData(), QIODevice::WriteOnly);

//            stream << (short)(sub->getLocation() - jump->getLocation() - 3);
//        }
//    }

//    // fix function calls
//    for (auto func : m_funcs)
//    {
//        for (auto call : func->getReferences())
//        {
//            unsigned short funcLoc = 0 | (func->getLocation() & 0xFFFF);

//            *(m_result.data() + call->getLocation() + 1) = ((funcLoc >> (8*1)) & 0xff);
//            *(m_result.data() + call->getLocation() + 2) = ((funcLoc >> (8*0)) & 0xff);

//            //TODO: verify this works
//        }
//    }

    return bytesWritten;
}

int Compiler::writeNatives()
{
    QByteArray natives;
    QDataStream str(&natives, QIODevice::WriteOnly);

    for (auto native : m_natives)
    {
        str << native;
    }

    while (natives.size() % 16 != 0 || natives.size() == 0)
    {
        natives.append(0xCD);
    }

    memcpy(m_result.data() + m_header.nativesPtr, natives.data(), natives.size());

    return natives.size();
}

int Compiler::writeStatics()
{
    QByteArray statics;
    QDataStream str(&statics, QIODevice::WriteOnly);

    for (auto staticVar : m_statics)
    {
        str << staticVar;
    }

    while (statics.size() % 16 != 0 || statics.size() == 0)
    {
        statics.append(0xCD);
    }

    memcpy(m_result.data() + m_header.staticsPtr, statics.data(), statics.size());

    return statics.size();
}

int Compiler::writeCodePageMap()
{
    QByteArray codePages;
    QDataStream str(&codePages, QIODevice::WriteOnly);

    for (auto ptr : m_codePageOffsets)
    {
        str << (0x50000000 | ptr);
    }

    codePages.append(roundUp(m_header.codePagesSize * 4, 16) - codePages.size(), 0xCD);

    memcpy(m_result.data() + m_header.codePagesPtr, codePages.data(), codePages.size());

    return codePages.size();
}

int Compiler::writePageMap()
{
    QByteArray pageMap;

    pageMap.append(4, 0);

    pageMap.append(roundUp(m_header.codePagesSize * 4 + 16, 16) - 4, 0xCD);

    memcpy(m_result.data() + m_header.pageMapPtr, pageMap.data(), pageMap.size());

    return pageMap.size();
}

void Compiler::writeHeader()
{
    QByteArray header;
    QDataStream stream(&header, QIODevice::WriteOnly);

    stream << 0xA8D74300;
    stream << (0x50000000 | m_header.pageMapPtr);
    stream << (0x50000000 | m_header.codePagesPtr);
    stream << m_header.codeSize;
    stream << m_header.paramCount;
    stream << m_statics.size();
    stream << (0x50000000 | m_header.staticsPtr);
    stream << m_header.globalsVers;
    stream << m_natives.size();
    stream << (0x50000000 | m_header.nativesPtr);

    header.append(8, 0xCD);

    memcpy(m_result.data() + m_header.headerPos, header.data(), header.size());
}
