#include "Application.h"

int main(int argc, char* argv[]) {
    if(argc < 2) return 1;

    Application app;
    return app.run(argv[1]);
}

