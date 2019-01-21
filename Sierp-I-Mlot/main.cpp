//   __   ______  _    _                
//   \ \ / / __ \| |  | |               
//    \ V / |  | | |__| | ___ _ __ ___  
//     > <| |  | |  __  |/ _ \ '__/ _ \ 
//    / . \ |__| | |  | |  __/ | | (_) |
//   /_/ \_\____/|_|  |_|\___|_|  \___/ 
//                                      
//                                      
//    _             _____                   _                   _    ____      _              
//   | |           |  __ \                 | |                 | |  / __ \    | |             
//   | |__  _   _  | |__) |_ ___      _____| |   __ _ _ __   __| | | |  | |___| | ____ _ _ __ 
//   | '_ \| | | | |  ___/ _` \ \ /\ / / _ \ |  / _` | '_ \ / _` | | |  | / __| |/ / _` | '__|
//   | |_) | |_| | | |  | (_| |\ V  V /  __/ | | (_| | | | | (_| | | |__| \__ \   < (_| | |   
//   |_.__/ \__, | |_|   \__,_| \_/\_/ \___|_|  \__,_|_| |_|\__,_|  \____/|___/_|\_\__,_|_|   
//           __/ |                                                                            
//          |___/                                                                             


#include "pch.h"
#include "XOConsoleApplication.h"

int main()
{
	//CHKERR_DWORD(Pa_Initialize());

	auto xo_application = new xo::XOConsoleApplication();
	xo_application->initialize();
	xo_application->run();

	return 0;
}
