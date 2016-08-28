#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_LEAD PT_LEAD 177 //You must change this line, replace LEAD with your element's name and 169 with the first available id in generated/ElementClasses.h
Element_LEAD::Element_LEAD() //Replace all the LEAD's below too
{
    Identifier = "DEFAULT_PT_LEAD";				//Identifier, used for lua and internal stuff, change LEAD to your element's name
    Name = "LEAD";								//Name, it is recommended to use 4 letters, but less works. 5 or more will probably not fit on the buttons
    Colour = PIXPACK(#778899);					//Default color
    MenuVisible = 1;							//If it's visible in the menu, if 0 then it is hidden and can only be produced with reactions or lua
    MenuSection = SC_SOLID;					//The menu section it's in, see src/simulation/SimulationData.h for a list, but you can probably guess their names on your own
    Enabled = 1;								//If 0, it cannot be created or seen in any way, not even lua. It will just disappear if somehow created. Used for removed elements, you should leave this 1
    
    Advection = 0.0f;							//How much the particle is accelerated by moving air. Normally 0 for solids, and up to 1 for other elements. It can be negative, ANAR and DEST do this so it goes towards pressure
    AirDrag = 0.00f * CFDS;						//How much air the particle generates in the direction of travel. Generally is very small, 0.04f creates a lot of positive air (- creates negative pressure).
    AirLoss = 0.90f;							//How much the particle slows down moving air (although not as big an effect as a wall). 1 = no effect, 0 = maximum effect. Solids are generally above 0.90f, along with most other elements too
    Loss = 0.00f;								//How much velocity the particle loses each frame. 1 = no loss, .5 = half loss. Solids have it at 0. Only a few have it at 1, like energy particles, and old moving sponge.
    Collision = 0.0f;							//Velocity is multiplied by this when the particle collides with something. Energy particles have it at -0.99f, everything else is -0.01f or 0.0f. This property does not do much at all.
    Gravity = 0.0f;								//How fast the particle falls. A negative number means it floats. Generally very small, most gasses are negative, everything else is usually less than 0.04f
    Diffusion = 0.00f;							//How much the particle "wiggles" around (think GAS or HYGN). Set at 0, except for gasses, which is a positive number. Up to 3 (or higher) for a large amount of wiggle, GAS is 0.75f, HYGN is 3.00f
    HotAir = 0.000f	* CFDS;						//How much the particle increases the pressure by. Another property only for gasses, but VENT/VACU have theirs at (-)0.010f. An extremely small number, sometimes as small as 0.000001f
    Falldown = 0;								//How does the particle move? 0 = solid, gas, or energy particle, 1 = powder, 2 = liquid.
    
    Flammable = 0;								//Does it burn? 0 = no, higher numbers = higher "burnage". Something like 20 is WOOD, while C-4 is 1000. Some are a few thousand for almost instant burning.
    Explosive = 0;								//Does it explode? 0 = no, 1 = when touching fire, 2 = when touching fire or when pressure > 2.5. Yes, those are the only options, see FIRE.cpp or somewhere in Simulation.cpp to modify how they work
    Meltable = 1;								//Does it melt? 1 or higher = yes, 0 = no. This is actually only used in legacy mode, not paying attention to this property once caused a bug where many solids that don't melt would without heat sim on.
    Hardness = 2;								//How much does acid affect it? 0 = no effect, higher numbers = higher effect. Generally goes up to about 50.
    
    Weight = 100;								//Heavier elements sink beneath lighter ones. 1 = Gas. 2 = Light, 98 = Heavy (liquids 0-49, powder 50-99). 100 = Solid. -1 is Neutrons and Photons.
    
    Temperature = R_TEMP+273.15f;				//What temperature does it have when created? Temperature is in Kelvin (Kelvin = degrees C + 273.15). R_TEMP+273.15f gives room temperature.
    HeatConduct = 251;							//0 - no heat transfer, 255 - maximum heat transfer speed.
    Description = "Lead,absorbs Protons and reflect Neutrons";	//A short one sentence description of the element, shown when you mouse over it in-game.
    
    State = ST_NONE;							//What state is this element? Options are ST_NONE, ST_SOLID, ST_LIQUID, ST_GAS. Powders use solid, energy particles and some other random things use none
    Properties = TYPE_SOLID;					//Does this element have special properties? Properties are listed in src/simulation/Element.h, you at least need to have the correct state property. If you want it to conduct electricity, be sure to use both PROP_CONDUCTS and PROP_LIFE_DEC
    
    LowPressure = IPL;
    LowPressureTransition = NT;
    HighPressure = IPH;
    HighPressureTransition = NT;
    LowTemperature = ITL;
    LowTemperatureTransition = NT;
    HighTemperature = ITH;
    HighTemperatureTransition = NT;
    
    Update = &Element_LEAD::update;
    Graphics = &Element_LEAD::graphics;
}

//#TPT-Directive ElementHeader Element_LEAD static int update(UPDATE_FUNC_ARGS)
int Element_LEAD::update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry;
	for (rx = -1; rx < 2; rx++)
		for (ry = -1; ry < 2; ry++)
			if (BOUNDS_CHECK)
			{
				r = pmap[y+ry][x+rx];
				if (!r || (r&0xFF) == PT_LEAD)
					r = sim->photons[y+ry][x+rx];
				if (!r)
					continue;
				if (parts[r>>8].temp + (parts[r>>8].temp*0.2f) <= MAX_TEMP)
				{
					parts[r>>8].temp += parts[r>>8].temp*0.2f;
				}
				else 
				{
					parts[r>>8].temp = MAX_TEMP;
				}
			}
    return 0;
}

//#TPT-Directive ElementHeader Element_LEAD static int graphics(GRAPHICS_FUNC_ARGS)
int Element_LEAD::graphics(GRAPHICS_FUNC_ARGS)
{
	return 1;
}

Element_LEAD::~Element_LEAD() {}
