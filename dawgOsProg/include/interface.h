
#include "display/lv_themes/lv_theme_templ_code.h"
#include "display/lv_themes/lv_theme_default_code.h"
// #include "display/lv_themes/lv_theme_alien_code.h"
#include "display/lv_themes/lv_theme_night_code.h"
#include "display/lv_themes/lv_theme_mono_code.h"
#include "display/lvgl.h"
#include "api.h"
#include <string>
// static  lv_res_t btn_rel_action(lv_obj_t * btn);
// static  lv_res_t ddlist_action(lv_obj_t * ddlist);
// static void btn_rel_action (void){
//     return;
// }
// static void ddlist_action (void){
//     return;
// }
LV_IMG_DECLARE(DogLogo);

void lv_tutorial_objects(void)
{
    // TEMPORARY USE ONLY
    pros::Controller master(pros::E_CONTROLLER_MASTER);
    
    lv_theme_t * th = lv_theme_night_init(65, NULL);
    
    /*Set the surent system theme*/
    lv_theme_set_current(th);
    
    // Loads main screen
    lv_obj_t * scr1 = lv_page_create(NULL, NULL);
    lv_scr_load(scr1);

    // Create Logo and put in top left corner of main screen
    lv_obj_t * logo = lv_img_create(scr1, NULL);
    lv_img_set_src(logo, &DogLogo);
    lv_page_glue_obj(logo, true);
    lv_obj_align(logo, scr1, LV_ALIGN_IN_TOP_LEFT,5,5);
    lv_obj_set_protect(logo, LV_PROTECT_POS);
    
    //Create bars for robot and controller battery and set it
    int robotCapacity   = 14500;
    int controlCapacity = 4200;

    lv_obj_t * robotBatt = lv_bar_create(scr1, NULL);
    lv_bar_set_range(robotBatt, 0, robotCapacity);
    lv_bar_set_value(robotBatt, pros::battery::get_voltage());
    lv_obj_set_size(robotBatt, 140,30);
    lv_obj_align(robotBatt, logo, LV_ALIGN_OUT_RIGHT_TOP,20,0);
    lv_page_glue_obj(robotBatt, true);
    lv_obj_set_protect(robotBatt, LV_PROTECT_POS);
    

    lv_obj_t * controlBatt = lv_bar_create(scr1, NULL);
    lv_bar_set_range(controlBatt, 0, controlCapacity);
    // printf("%d", master.get_battery_level());
    lv_bar_set_value(controlBatt, master.get_battery_level());
    lv_obj_set_size(controlBatt, 140,30);
    lv_obj_align(controlBatt, robotBatt, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);
    lv_obj_set_protect(controlBatt, LV_PROTECT_POS);
    

    lv_obj_t * robotBattLabel = lv_label_create(robotBatt, NULL);
    lv_label_set_text(robotBattLabel, "Robot");
    lv_label_set_align(robotBattLabel,LV_LABEL_ALIGN_CENTER);
    lv_obj_align(robotBattLabel, robotBatt, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * controlBattLabel = lv_label_create(controlBatt, NULL);
    lv_label_set_text(controlBattLabel, "Controller");
    lv_label_set_align(controlBattLabel,LV_LABEL_ALIGN_CENTER);
    lv_obj_align(controlBattLabel, controlBatt, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * testBtn = lv_btn_create(scr1, NULL);
    lv_obj_set_hidden(testBtn, true);
    static lv_style_t red_btn_tgl_rel;
    static lv_style_t red_btn_tgl_pr;
    static lv_style_t red_btn_rel;
    lv_style_copy(&red_btn_tgl_pr, lv_btn_get_style(testBtn, LV_BTN_STYLE_TGL_PR));
    lv_style_copy(&red_btn_tgl_rel, lv_btn_get_style(testBtn, LV_BTN_STYLE_TGL_REL));
    lv_style_copy(&red_btn_rel, lv_btn_get_style(testBtn, LV_BTN_STYLE_REL));
    lv_obj_del(testBtn);

    red_btn_tgl_rel.body.main_color = LV_COLOR_RED;
    red_btn_tgl_rel.body.grad_color = LV_COLOR_MAKE(0xa0, 0x00, 0x00);
    red_btn_tgl_pr.body.main_color = LV_COLOR_MAKE(0xa0, 0x00, 0x00);
    red_btn_tgl_pr.body.grad_color = LV_COLOR_MAKE(0xa0, 0x00, 0x00);
    red_btn_rel.body.border.color = LV_COLOR_RED;
    red_btn_rel.body.border.width = 2;
    
    static lv_style_t blue_btn_tgl_rel;
    static lv_style_t blue_btn_tgl_pr;
    static lv_style_t blue_btn_rel;
    lv_style_copy(&blue_btn_tgl_pr, &red_btn_tgl_pr);
    lv_style_copy(&blue_btn_tgl_rel, &red_btn_tgl_rel);
    lv_style_copy(&blue_btn_rel, &red_btn_rel);

    blue_btn_tgl_rel.body.main_color = LV_COLOR_BLUE;
    blue_btn_tgl_rel.body.grad_color = LV_COLOR_MAKE(0x00, 0x3f, 0xb5);
    blue_btn_tgl_pr.body.main_color = LV_COLOR_MAKE(0x00, 0x3f, 0xb5);
    blue_btn_tgl_pr.body.grad_color = LV_COLOR_MAKE(0x00, 0x3f, 0xb5);
    blue_btn_rel.body.border.color = LV_COLOR_BLUE;
    blue_btn_rel.body.border.width = 2;


    lv_obj_t * autonRedBtn = lv_btn_create(scr1,NULL);
    lv_btn_set_toggle(autonRedBtn,true);
    lv_obj_set_size(autonRedBtn,100,75);
    lv_btn_set_style(autonRedBtn, LV_BTN_STYLE_TGL_PR, &red_btn_tgl_pr);
    lv_btn_set_style(autonRedBtn, LV_BTN_STYLE_TGL_REL, &red_btn_tgl_rel);
    lv_btn_set_style(autonRedBtn, LV_BTN_STYLE_REL, &red_btn_rel);
    lv_obj_align(autonRedBtn, robotBatt, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t * autonBlueBtn = lv_btn_create(scr1,autonRedBtn);
    lv_btn_set_style(autonBlueBtn, LV_BTN_STYLE_TGL_PR, &blue_btn_tgl_pr);
    lv_btn_set_style(autonBlueBtn, LV_BTN_STYLE_TGL_REL, &blue_btn_tgl_rel);
    lv_btn_set_style(autonBlueBtn, LV_BTN_STYLE_REL, &blue_btn_rel);
    lv_obj_align(autonBlueBtn, controlBatt, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t * redLabel = lv_label_create(autonRedBtn, NULL);
    lv_label_set_text(redLabel, "RED");
    lv_label_set_align(redLabel,LV_LABEL_ALIGN_CENTER);
    lv_obj_align(redLabel, autonRedBtn, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * blueLabel = lv_label_create(autonBlueBtn, NULL);
    lv_label_set_text(blueLabel, "BLUE");
    lv_label_set_align(blueLabel,LV_LABEL_ALIGN_CENTER);
    lv_obj_align(blueLabel, autonBlueBtn, LV_ALIGN_CENTER, 0, 0);

    static lv_point_t line_points[] = { {0, 0}, {1, 1}};
    lv_obj_t * line1;
    line1 = lv_line_create(lv_scr_act(), NULL);
    lv_line_set_points(line1, line_points, 5);     /*Set the points*/
    lv_obj_align(line1, controlBatt, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
    lv_obj_set_hidden(line1, true);
    int btnmXMax = lv_obj_get_x(line1);
    lv_obj_align(line1, robotBatt, LV_ALIGN_OUT_LEFT_BOTTOM, 0, 0);
    int btnmXMin = lv_obj_get_x(line1);
    int btnmWidth = btnmXMax - btnmXMin;
    lv_obj_align(line1, autonRedBtn, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    int btnmY = lv_obj_get_y(line1) + 20;
    lv_obj_del(line1);
    
    static const char * autonTypes[] = {"1", "2", "3", "4", ""};

    lv_obj_t * autonTypeBtnm = lv_btnm_create(scr1, NULL);
    lv_btnm_set_map(autonTypeBtnm, autonTypes);
    lv_btnm_set_toggle(autonTypeBtnm, true, 0);
    // lv_obj_align(autonTypeBtnm, autonRedBtn, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_set_x(autonTypeBtnm, btnmXMin);
    lv_obj_set_y(autonTypeBtnm, btnmY);
    lv_obj_set_size(autonTypeBtnm, btnmWidth, 50);

    // // lv_obj_t * autonTypeBtnm = lv_btnm_create(scr1, NULL);
    
    // while(true){
    //     lv_bar_set_value(robotBatt, pros::battery::get_voltage());
        
    //     lv_bar_set_value(controlBatt, master.get_battery_level());
        
    //     robotPercent = (int) (pros::battery::get_voltage()/robotCapacity)*100;
    //     robotStr = "Robot: " + std::to_string(robotPercent) + "\0";
    //     const char * robotChar = robotStr.c_str();
    //     lv_label_set_text(robotBattLabel, robotChar);
    //     delete[] robotChar;
        
    //     controlPercent = (int) (master.get_battery_level()/controlCapacity)*100;
    //     controlStr = "Control: " + std::to_string(controlPercent) + "\0";
    //     const char * controlChar = robotStr.c_str();
    //     lv_label_set_text(controlBattLabel, controlChar);
    //     delete[] controlChar;

        

    //     // for(x; x<=300; x++){
    //     //     lv_obj_set_x(logo, x);
    //     //     pros::delay(15);
    //     // }
    //     // for(x; x>=0; x--){
    //     //     lv_obj_set_x(logo, x);
    //     //     pros::delay(15);
    //     // }
    //     // if(x>=300 || x<=0){
    //     //     xDirection = 1-xDirection;
    //     //     // xRate = (rand() % 2) + 1;
    //     // }
    //     // if(y>=105 || y<=10){
    //     //     yDirection = 1-yDirection;
    //     //     // yRate = (rand() % 2) + 1;
    //     // }
    //     // if(xDirection == 1){
    //     //     x++;
    //     // }
    //     // else{
    //     //     x--;
    //     // }
    //     // if(yDirection == 1){
    //     //     y++;
    //     // }
    //     // else{
    //     //     y--;
    //     // }
    //     // lv_obj_set_x(logo, x);
    //     // lv_obj_set_y(logo, y);
    //     pros::delay(100);
    // }
}