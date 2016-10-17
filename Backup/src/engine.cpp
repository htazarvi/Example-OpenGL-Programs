
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

bool Engine::Initialize(char **argv)
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
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, argv))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

  eventFlags.push_back(EventFlag());

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
    m_graphics->Update(m_DT, eventFlags, viewUpdate);
    m_graphics->Render();

		//reset the view update type
		viewUpdate.type = NONE;

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
    // handle key down events here
    switch(m_event.key.keysym.sym){
      case SDLK_ESCAPE:
        m_running = false;
        break;

      case SDLK_LEFT:
        //'Left Arrow' pressed, change object rotation direction counter-clockwise
        if( eventFlags[0].clockwise_rotate == true )
          eventFlags[0].clockwise_rotate = false;
        break;

      case SDLK_RIGHT:
        //'Right Arrow' pressed, change object rotation direction clockwise
        if( eventFlags[0].clockwise_rotate == false )
          eventFlags[0].clockwise_rotate = true;
        break;

      case SDLK_PAUSE:
        //Pause or resume program
        if( eventFlags[0].pause_all == false )
        {
          eventFlags[0].pause_all = true;
        }

        else if( eventFlags[0].pause_all == true )
        { 
          eventFlags[0].pause_all = false;
        }

        break;

			case SDLK_w:
				//'W' pressed, move camera forward
				viewUpdate.type = EYE;
				viewUpdate.value = glm::vec3(0.0f, 0.0f, 5.0f);
				break;

			case SDLK_a:
				//'A' pressed, move camera left
				viewUpdate.type = EYE;
				viewUpdate.value = glm::vec3(-5.0f, 0.0f, 0.0f);
				break;

			case SDLK_s:
				//'S' pressed, move camera backward
				viewUpdate.type = EYE;
				viewUpdate.value = glm::vec3(0.0f, 0.0f, -5.0f);
				break;

			case SDLK_d:
				//'D' pressed, move camera right
				viewUpdate.type = EYE;
				viewUpdate.value = glm::vec3(5.0f, 0.0f, 0.0f);
				break;

      default:
        break;
    }
  }
}

void Engine::Mouse(){

  if (m_event.type == SDL_MOUSEBUTTONDOWN)
  {
    // handle mouse button down events here
    switch(m_event.button.button){
      case SDL_BUTTON_LEFT:
        //Left click, change object rotation direction counter-clockwise
        if( eventFlags[0].clockwise_rotate == true )
          eventFlags[0].clockwise_rotate = false;
        break;
      case SDL_BUTTON_RIGHT:
        //Right click, change object rotation direction clockwise
        if( eventFlags[0].clockwise_rotate == false )
          eventFlags[0].clockwise_rotate = true;
        break;
      default:
        break;
    }
  }

	//handle mouse look here
  if (m_event.type == SDL_MOUSEMOTION){
		if (m_event.motion.xrel < 0){
			viewUpdate.type = FOCUS;
			viewUpdate.value = glm::vec3(-15.0f, 0.0f, 0.0f);
		}

		if (m_event.motion.xrel > 0){
			viewUpdate.type = FOCUS;
			viewUpdate.value = glm::vec3(15.0f, 0.0f, 0.0f);
		}
	
		if (m_event.motion.yrel < 0){
			viewUpdate.type = FOCUS;
			viewUpdate.value = glm::vec3(0.0f, -15.0f, 0.0f);
		}

		if (m_event.motion.yrel > 0){
			viewUpdate.type = FOCUS;
			viewUpdate.value = glm::vec3(0.0f, 15.0f, 0.0f);
		}	
  }

	//handle zoom
	if (m_event.type == SDL_MOUSEWHEEL)
	{
		if (m_event.wheel.y < 0){
			printf("wheel down\n");
		}
		
		if (m_event.wheel.y > 0){
			printf("wheel up\n");
		}
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