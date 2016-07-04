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
#define UCI_CFG_MAX_LEN 128



class Constants
{
public:
    Constants();
    int load_uci_config_string(std :: string uci_path, std::string uci_cmd, std::string * string_to_set);
    int load_uci_config_int(std :: string uci_path, std::string uci_cmd, int * num_to_set);


    int particle_number;
    int circus;
    int zoom_scale;
    int wifi_room_check_threshold;
    int delay_time_val;
    std::string allowed_list_path;
    std::string room_list_path;
    std::string map_file_path;

    

};

#endif /* defined(__mapTest__Constants__) */
