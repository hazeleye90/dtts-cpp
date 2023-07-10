
add_library(dtts_lib 
	"window.cpp" 
	"window.h" 
	"entity.cpp"
	"entity.h"
	"drawing.cpp"
	"drawing.h"
	"controller.cpp"
	"controller.h"
	"keyboard.cpp"
	"keyboard.h"
	"util.h"
	"text.cpp"
	"text.h"
	"ship.cpp"
	"ship.h"
	"timer.cpp"
	"timer.h"
	"level.cpp"
	"level.h"
	"levelFactory.cpp"
	"levelFactory.h"
	"scene.cpp"
	"scene.h"
	"levelScene.cpp"
	"levelScene.h"
	"titleScene.cpp"
	"titleScene.h"
	"actions.cpp"
	"actions.h"
	"collision.h"
	"collision.cpp"
	"constants.h"
)
add_library(dtts::lib ALIAS dtts_lib)
target_link_libraries(dtts_lib PUBLIC ${SDL2_LIBRARIES})
       
add_executable(dtts "main.cpp")
target_link_libraries(dtts PRIVATE dtts::lib)
