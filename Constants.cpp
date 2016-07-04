//
// Created by zhangzhao on 16-7-3.
//

#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <syslog.h>
#include "Constants.h"
#include <iostream>


Constants::Constants() {
    std:: string file_dir = "./params/";
    load_uci_config_string(file_dir, "config.paths.allowed_list_path", &(this->allowed_list_path));
    load_uci_config_string(file_dir, "config.paths.room_list_path", &(this->room_list_path));
    load_uci_config_string(file_dir, "config.paths.map_file_path", &(this->map_file_path));

    load_uci_config_int(file_dir, "config.vars.particle_number", &(this->particle_number));
    load_uci_config_int(file_dir, "config.vars.circus", &(this->circus));
    load_uci_config_int(file_dir, "config.vars.zoom_scale", &(this->zoom_scale));
    load_uci_config_int(file_dir, "config.vars.wifi_room_check_threshold", &(this->wifi_room_check_threshold));
    load_uci_config_int(file_dir, "config.vars.delay_time_val", &(this->delay_time_val));


}

int Constants:: load_uci_config_string(std :: string uci_path, std::string uci_cmd, std::string * string_to_set)
{
    std::string final_cmd = "uci -c " + uci_path + " get " + uci_cmd;
    std::cout<<final_cmd<<std::endl;
    FILE * fp;
    char result [UCI_CFG_MAX_LEN];
    memset(result, 0, UCI_CFG_MAX_LEN);
    fp = popen(final_cmd.c_str(),"r");
    fscanf(fp," %[^\n]s", result);
    pclose(fp);
    if(strlen(result) == 0)
        return 1;
    *string_to_set = result;
    return 0;
}

int Constants:: load_uci_config_int(std :: string uci_path, std::string uci_cmd, int * num_to_set)
{
    std::string result_str;
    int status = load_uci_config_string(uci_path, uci_cmd, &result_str);
    if(status == 1)
    {
        return 1;
    }
    else{
        *num_to_set = std:: stoi (result_str,nullptr,10);
        return 0;
    }
}