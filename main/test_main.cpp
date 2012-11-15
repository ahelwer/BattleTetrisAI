#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cstdlib>

int main() {
    srand(time(NULL));
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry &registry = 
        CppUnit::TestFactoryRegistry::getRegistry();
    runner.addTest(registry.makeTest());
    return runner.run("", false);
}

