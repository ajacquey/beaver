#include "BeaverApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
BeaverApp::validParams()
{
  InputParameters params = MooseApp::validParams();

  // Do not use legacy material output
  params.set<bool>("use_legacy_material_output") = false;

  return params;
}

BeaverApp::BeaverApp(InputParameters parameters) : MooseApp(parameters)
{
  BeaverApp::registerAll(_factory, _action_factory, _syntax);
}

BeaverApp::~BeaverApp() {}

void
BeaverApp::registerAll(Factory & f, ActionFactory & af, Syntax & syntax)
{
  ModulesApp::registerAllObjects<BeaverApp>(f, af, syntax);
  Registry::registerObjectsTo(f, {"BeaverApp"});
  Registry::registerActionsTo(af, {"BeaverApp"});

  /* register custom execute flags, action syntax, etc. here */
  registerSyntax("EmptyAction", "BCs/BVPressure");
  registerSyntax("BVPressureAction", "BCs/BVPressure/*");
  registerSyntax("EmptyAction", "BCs/BVShearTraction");
  registerSyntax("BVShearTractionAction", "BCs/BVShearTraction/*");
}

void
BeaverApp::registerApps()
{
  registerApp(BeaverApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
BeaverApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  BeaverApp::registerAll(f, af, s);
}
extern "C" void
BeaverApp__registerApps()
{
  BeaverApp::registerApps();
}
