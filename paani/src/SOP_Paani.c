//
//  SOP_Paani.C
//  paani
//
//  Created by Debanshu on 3/20/15.
//  Copyright (c) 2015 Debanshu. All rights reserved.
//

#include "SOP_Paani.h"

#include <GU/GU_Detail.h>
#include <GU/GU_PrimSphere.h>
#include <OP/OP_Operator.h>
#include <OP/OP_OperatorTable.h>
#include <PRM/PRM_Include.h>
#include <CH/CH_LocalVariable.h>
#include <UT/UT_DSOVersion.h>
#include <UT/UT_Interrupt.h>
#include <SYS/SYS_Math.h>
#include <limits.h>
#include <GEO/GEO_PrimType.h>

using namespace HDK_Sample;
Scene* scene;
//
// Help is stored in a "wiki" style text file.  This text file should be copied
// to $HOUDINI_PATH/help/nodes/sop/star.txt
//
// See the sample_install.sh file for an example.
//


///
/// newSopOperator is the hook that Houdini grabs from this dll
/// and invokes to register the SOP.  In this case we add ourselves
/// to the specified operator table.
///
void
newSopOperator(OP_OperatorTable *table)
{
    table->addOperator(new OP_Operator(
                                       "hdk_paani",                 // Internal name
                                       "Paani",                     // UI name
                                       SOP_Paani::myConstructor,    // How to build the SOP
                                       SOP_Paani::myTemplateList,   // My parameters
                                       0,                          // Min # of sources
                                       0,                          // Max # of sources
                                       SOP_Paani::myVariables,      // Local variables
                                       OP_FLAG_GENERATOR));        // Flag it as generator
}

static PRM_Name		particleCountName("particleCount", "ParticleCount");
static PRM_Default	particleCountDefault(500);

PRM_Template
SOP_Paani::myTemplateList[] = {
    PRM_Template(PRM_INT,	PRM_Template::PRM_EXPORT_MIN, 1, &particleCountName, &particleCountDefault, 0),
//    PRM_Template(PRM_STRING,	PRM_Template::PRM_EXPORT_MIN, 1, &grammar, &grammarDefault, 0),
//        PRM_Template(PRM_FLT,	PRM_Template::PRM_EXPORT_MIN, 1, &stepName, &stepDefault, 0),
    PRM_Template()
};


// Here's how we define local variables for the SOP.
enum {
    VAR_PT,		// Point number of the star
    VAR_NPT		// Number of points in the star
};

CH_LocalVariable
SOP_Paani::myVariables[] = {
    { "PT",	VAR_PT, 0 },		// The table provides a mapping
    { "NPT",	VAR_NPT, 0 },		// from text string to integer token
    { 0, 0, 0 },
};

bool
SOP_Paani::evalVariableValue(fpreal &val, int index, int thread)
{
    // myCurrPoint will be negative when we're not cooking so only try to
    // handle the local variables when we have a valid myCurrPoint index.
    if (myCurrPoint >= 0)
    {
        // Note that "gdp" may be null here, so we do the safe thing
        // and cache values we are interested in.
        switch (index)
        {
            case VAR_PT:
                val = (fpreal) myCurrPoint;
                return true;
            case VAR_NPT:
                val = (fpreal) myTotalPoints;
                return true;
            default:
                /* do nothing */;
        }
    }
    // Not one of our variables, must delegate to the base class.
    return SOP_Node::evalVariableValue(val, index, thread);
}

OP_Node *
SOP_Paani::myConstructor(OP_Network *net, const char *name, OP_Operator *op)
{
    return new SOP_Paani(net, name, op);
}

SOP_Paani::SOP_Paani(OP_Network *net, const char *name, OP_Operator *op)
: SOP_Node(net, name, op)
{
    // This SOP always generates fresh geometry, so setting this flag
    // is a bit redundant, but one could change it to check for the old
    // star and only bump relevant data IDs, (P and the primitive list),
    // depending on what parameters changed.
    mySopFlags.setManagesDataIDs(true);

    scene = new Scene();
    scene->init();

    myCurrPoint = -1; // To prevent garbage values from being returned
}

SOP_Paani::~SOP_Paani() {}

OP_ERROR
SOP_Paani::cookMySop(OP_Context &context)
{
    flags().setTimeDep(1);
    fpreal now = context.getTime();
    
    // Since we don't have inputs, we don't need to lock them.
//    int particleCount = COUNT(now);

//    scene->numberOfParticles = particleCount;
    scene->update();
    // now the updated particles should be in vector<Particle> scene->getAllParticles()
    
    int divisions  = DIVISIONS(now)*2;  // We need twice our divisions of points
    
    // Check to see that there hasn't been a critical error in cooking the SOP.
    if (error() >= UT_ERROR_ABORT)
    {
        myCurrPoint = -1;
        return error();
    }
    
    if (divisions < 4)
    {
        // With the range restriction we have on the divisions, this
        // is actually impossible, but it shows how to add an error
        // message or warning to the SOP.
        addWarning(SOP_MESSAGE, "Invalid divisions");
        divisions = 4;
    }
    
    // In addition to destroying everything except the empty P
    // and topology attributes, this bumps the data IDs for
    // those remaining attributes, as well as the primitive list
    // data ID.
    gdp->clearAndDestroy();
    
    // Start the interrupt server
    UT_AutoInterrupt boss("Building Paani");
    if (boss.wasInterrupted())
    {
        myCurrPoint = -1;
        return error();
    }
    
    
    std::vector<Particle> particles = scene->particleSystem->getAllParticles();
    for (std::vector<Particle>::iterator it=particles.begin(); it < particles.end(); it++)
    {
        // Check to see if the user has interrupted us...
        if (boss.wasInterrupted())
            break;
        
        GU_PrimSphereParms sphereparms;
        sphereparms.gdp		= gdp;		// geo detail to append to
        sphereparms.xform.scale(0.5, 0.5, 0.5);	// set the radii

        // Build a sphere instead of this poly
        Particle particle = *it;
        glm::vec3 newPos = particle.getPosition();
        sphereparms.xform.translate(newPos[0],newPos[1],newPos[2]);
        
        GEO_Primitive *sphere;
        sphere = GU_PrimSphere::build(sphereparms, GEO_PRIMSPHERE);

        
//        GA_Offset ptoff = poly->getPointOffset(i);
//        gdp->setPos3(ptoff, pos);
    }

    // Highlight the star which we have just generated.  This routine
    // call clears any currently highlighted geometry, and then it
    // highlights every primitive for this SOP.
    select(GU_SPrimitive);
    
    myCurrPoint = -1;
    return error();
}