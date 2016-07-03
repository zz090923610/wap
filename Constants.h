//
//  Constants.h
//  mapTest
//
//  Created by 张 钊 on 9/9/13.
//
//
#include <string>
#ifndef __mapTest__Constants__
#define __mapTest__Constants__

#define UNDEFINEDSTATUS 0
#define APPROACHING 1
#define LEAVING -1
#define APPEAR 2
#define DISAPPEAR -2




class Constants
{
public:
    Constants();
    Constants(std::string file_path);

    int particle_number;
    int start_pos_x, start_pos_y;
    int circus;
    int zoom_scale;
    int wifi_room_check_threshold;
    int delay_time_val;
    std::string allowed_list_path;
    std::string room_list_path;
    std::string map_file_path;

    

};

#endif /* defined(__mapTest__Constants__) */
