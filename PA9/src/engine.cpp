
#include "engine.hpp"

Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
}

Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize(char *configFile)
{
  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, configFile))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();


  bool selectAmbient = false;
  bool selectDiffuse = false;
  bool selectSpecular = false;

  // No errors
  return true;
}

void Engine::Run()
{
  m_running = true;

  while(m_running)
  {
    // Update the DT
    m_DT = getDT();

    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      Keyboard();
      Mouse();
    }

    // Update and render the graphics
    m_graphics->Update(m_DT, &m_event);
    m_graphics->Render();

    // Swap to the Window
    m_window->Swap();
  }
}

void Engine::Keyboard()
{
  if(m_event.type == SDL_QUIT)
  {
    m_running = false;
  }
  else if (m_event.type == SDL_KEYDOWN)
  {
    float sclStepDown = 0.80;
    float sclStepUp = 1.25;
    // handle key down events here
    switch(m_event.key.keysym.sym)
    {
      case SDLK_ESCAPE:
        m_running = false;
        break;
      case SDLK_1:
        //'1' pressed, use per fragment lighting
        m_graphics->SetPerFragLighting();
        if (!m_graphics->SetShader())
          printf("Failed to switch shader for per fragment lighting\n");
        break;
      case SDLK_2:
        //'2' pressed, use per vertex lighting
        m_graphics->SetPerVertLighting();
        if (!m_graphics->SetShader())
          printf("Failed to switch shader for per vertex lighting\n");
        break;
      case SDLK_a:
        selectAmbient = true;
        selectDiffuse = false;
        selectSpecular = false;
        break;
      case SDLK_d:
        selectAmbient = false;
        selectDiffuse = true;
        selectSpecular = false;
        break;
      case SDLK_s:
        selectAmbient = false;
        selectDiffuse = false;
        selectSpecular = true;
        break;       
      case SDLK_EQUALS:
        if(selectAmbient)
          m_graphics->SetAmbientScalar(m_graphics->getAmbientScalar()*sclStepUp);
        else if(selectDiffuse)
          m_graphics->SetDiffuseScalar(m_graphics->getDiffuseScalar()*sclStepUp);
        else if(selectSpecular)
          m_graphics->SetSpecularScalar(m_graphics->getSpecularScalar()*sclStepUp);
        break;
      case SDLK_MINUS:
        if(selectAmbient)
          m_graphics->SetAmbientScalar(m_graphics->getAmbientScalar()*sclStepDown);
        else if(selectDiffuse)
          m_graphics->SetDiffuseScalar(m_graphics->getDiffuseScalar()*sclStepDown);
        else if(selectSpecular)
          m_graphics->SetSpecularScalar(m_graphics->getSpecularScalar()*sclStepDown);
        break;
      default:
        break;
    }
  }
}

void Engine::Mouse(){

	//handle mouse look here
  if (m_event.type == SDL_MOUSEMOTION){

  }

}

unsigned int Engine::getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}