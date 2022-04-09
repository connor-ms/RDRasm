#include "types.h"
#include "constants.h"
#include "intrinsics.h"
#include "natives.h"
#include "common.h"

#define self GET_PLAYER_ACTOR(0)

int sumExample(int val1, int val2)
{
	return val1 + val2;
}

// Example function to teleport the player 2ft in a certain direction
void teleportInDirection(int teleport_dir)
{
    vector3 loc;
    GET_POSITION(self, &loc);

    float heading = GET_HEADING(self);

    float dist = 2.0;

    float a = dist * SIN(heading);
    float b = dist * COS(heading);

    switch (teleport_dir)
    {
    case 0: // forward
        loc.x -= a;
        loc.z -= b;
        break;
    case 1: // back
        loc.x += a;
        loc.z += b;
        break;
    case 2: // up
        loc.y += 2.0;
        break;
    case 3: // down
        loc.y -= 2.0;
        break;
    }

    TELEPORT_ACTOR_WITH_HEADING(self, loc, heading, true, true, true);
}

void main()
{
	float f1 = 0.1f;
	float f2 = 9999.9f;
	
	int i1 = 100;
	int i2 = 10000;

	int sum = sumExample(i1, i2);

	teleportInDirection(0);

	print("Example print message for 10 seconds", i2);
}