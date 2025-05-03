
#include "../Visualizer.hpp"

#ifdef __unix__

int main()
{
    TVis::Visualizer visualizer;

    visualizer.Init();

    visualizer.Update();
}

#else

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    TVis::Visualizer visualizer;
    
    visualizer.Init();
    
    visualizer.Update();

    return EXIT_SUCCESS;
}

#endif