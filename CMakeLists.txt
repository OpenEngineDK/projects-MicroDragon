# Project name. Should be the same as the project directory name and
# will become the name of the binary target.
SET( PROJECT_NAME "DragonPanic")

# Project source code list
SET( PROJECT_SOURCES
  # Add all the cpp source files here
  main.cpp
  GameFactory.cpp
  KeyHandler.cpp
  LightFader.cpp

  Common/utilities.cpp
  Common/OpenGLUtil.cpp
  Common/FaceListUtil.cpp
  Common/Vec3.cpp
  Common/random.cpp
  Common/Follower.cpp

  Modules/Island/Island.cpp
  Modules/OscSurface/OscSurface.cpp
  Modules/Intro/Intro.cpp
  Modules/Target/Target.cpp
  Modules/InputGrabber/InputGrabber.cpp

  Modules/Boid/Boid.cpp
  Modules/Boid/BoidsSystem.cpp

  Modules/Particle/ParticleSystem.cpp
  Modules/Particle/Particle.cpp
  Modules/Particle/FireBall.cpp

  Modules/Dragon/Dragon.cpp
  Modules/Dragon/Tube.cpp
)

# Include needed to use SDL under Mac OS X
IF(APPLE)
  SET(PROJECT_SOURCES ${PROJECT_SOURCES}  ${SDL_MAIN_FOR_MAC})
ENDIF(APPLE)

# Project executable
ADD_EXECUTABLE(${PROJECT_NAME}
  ${PROJECT_SOURCES}
)

# Project dependencies
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
  # Core library dependencies
  OpenEngine_Core
  OpenEngine_Logging
  OpenEngine_Display
  OpenEngine_Resources
  OpenEngine_Devices
  OpenEngine_Renderers_OpenGL
  OpenEngine_Scene
  OpenEngine_Utils
  # Extension dependencies
  Extensions_GenericHandlers
  Extensions_OBJResource
  Extensions_TGAResource
  Extensions_SDL
)
