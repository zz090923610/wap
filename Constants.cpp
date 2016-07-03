//
// Created by zhangzhao on 16-7-3.
//

#include "Constants.h"

Constants::Constants() {
    this -> particle_number = 4000;
    this -> start_pos_x = 750;
    this -> start_pos_y = 40;
    this ->circus = 240;
    this -> zoom_scale = 10;
    this -> wifi_room_check_threshold = -50;
    this -> delay_time_val = 24;

    this ->allowed_list_path = "./allowedList.txt";
    this -> room_list_path ="./params/roomList.txt";
    this -> map_file_path ="./params/mapData.txt";
}
Constants::Constants(std::string file_path) {

}