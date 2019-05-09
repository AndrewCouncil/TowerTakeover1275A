
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
int activeScr = 0;

// DEFINE ALL OBJECTS TO USE OUTSIDE OF MAIN CODE

lv_theme_t * th = lv_theme_night_init(65, NULL);

// SCREENS
lv_obj_t * scr2 = lv_page_create(NULL, NULL);
lv_obj_t * scr1 = lv_page_create(NULL, NULL);
lv_obj_t * scr0 = lv_page_create(NULL, NULL);

// SCREEN 0 OBJECTS
lv_obj_t * robotBatt = lv_bar_create(scr0, NULL);
lv_obj_t * logo = lv_img_create(scr0, NULL);
lv_obj_t * controlBatt = lv_bar_create(scr0, NULL);
lv_obj_t * robotBattLabel = lv_label_create(robotBatt, NULL);
lv_obj_t * controlBattLabel = lv_label_create(controlBatt, NULL);
lv_obj_t * autonTypeBtnm = lv_btnm_create(scr0, NULL);

// SCREEN 1 OBJECTS


static lv_res_t setScr0Action(lv_obj_t * btn)
{
    uint8_t id = lv_obj_get_free_num(btn);
    lv_scr_load(scr0);
    return LV_RES_OK; /*Return OK if the button is not deleted*/
}
static lv_res_t setScr1Action(lv_obj_t * btn)
{
    uint8_t id = lv_obj_get_free_num(btn);
    lv_scr_load(scr1);
    return LV_RES_OK; /*Return OK if the button is not deleted*/
}
static lv_res_t setScr2Action(lv_obj_t * btn)
{
    uint8_t id = lv_obj_get_free_num(btn);
    lv_scr_load(scr2);
    return LV_RES_OK; /*Return OK if the button is not deleted*/
}

void lv_tutorial_objects(void)
{
    // SCREEN 0 OBJECTS
    
    /*Set the surent system theme*/
    lv_theme_set_current(th);
    
    // Loads main screen
    lv_scr_load(scr0);

    // Create Logo and put in top left corner of main screen
    lv_img_set_src(logo, &DogLogo);
    lv_page_glue_obj(logo, true);
    lv_obj_align(logo, scr0, LV_ALIGN_IN_TOP_LEFT,5,5);
    lv_obj_set_protect(logo, LV_PROTECT_POS);
    
    //Create bars for robot and controller battery and set it
    int robotCapacity   = 14500;
    int controlCapacity = 4200;
    
    lv_bar_set_range(robotBatt, 0, robotCapacity);
    lv_bar_set_value(robotBatt, pros::battery::get_voltage());
    lv_obj_set_size(robotBatt, 140,30);
    lv_obj_align(robotBatt, logo, LV_ALIGN_OUT_RIGHT_TOP,20,0);
    lv_page_glue_obj(robotBatt, true);
    lv_obj_set_protect(robotBatt, LV_PROTECT_POS);
    
    lv_bar_set_range(controlBatt, 0, controlCapacity);
    // printf("%d", master.get_battery_level());
    lv_bar_set_value(controlBatt, master.get_battery_level());
    lv_obj_set_size(controlBatt, 140,30);
    lv_obj_align(controlBatt, robotBatt, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);
    lv_obj_set_protect(controlBatt, LV_PROTECT_POS);
    
    // Label Battery Bars
    lv_label_set_text(robotBattLabel, "Robot");
    lv_label_set_align(robotBattLabel,LV_LABEL_ALIGN_CENTER);
    lv_obj_align(robotBattLabel, robotBatt, LV_ALIGN_CENTER, 0, 0);

    
    lv_label_set_text(controlBattLabel, "Controller");
    lv_label_set_align(controlBattLabel,LV_LABEL_ALIGN_CENTER);
    lv_obj_align(controlBattLabel, controlBatt, LV_ALIGN_CENTER, 0, 0);

    // Create a dummy button, copy its style, delete it
    lv_obj_t * testBtn = lv_btn_create(scr0, NULL);
    lv_obj_set_hidden(testBtn, true);
    static lv_style_t red_btn_tgl_rel;
    static lv_style_t red_btn_tgl_pr;
    static lv_style_t red_btn_rel;
    lv_style_copy(&red_btn_tgl_pr, lv_btn_get_style(testBtn, LV_BTN_STYLE_TGL_PR));
    lv_style_copy(&red_btn_tgl_rel, lv_btn_get_style(testBtn, LV_BTN_STYLE_TGL_REL));
    lv_style_copy(&red_btn_rel, lv_btn_get_style(testBtn, LV_BTN_STYLE_REL));
    lv_obj_del(testBtn);

    // Make styles for red and blue buttons
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

    // Make Red and Blue buttons
    lv_obj_t * autonRedBtn = lv_btn_create(scr0,NULL);
    lv_btn_set_toggle(autonRedBtn,true);
    lv_obj_set_size(autonRedBtn,100,75);
    lv_btn_set_style(autonRedBtn, LV_BTN_STYLE_TGL_PR, &red_btn_tgl_pr);
    lv_btn_set_style(autonRedBtn, LV_BTN_STYLE_TGL_REL, &red_btn_tgl_rel);
    lv_btn_set_style(autonRedBtn, LV_BTN_STYLE_REL, &red_btn_rel);
    lv_obj_align(autonRedBtn, robotBatt, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t * autonBlueBtn = lv_btn_create(scr0,autonRedBtn);
    lv_btn_set_style(autonBlueBtn, LV_BTN_STYLE_TGL_PR, &blue_btn_tgl_pr);
    lv_btn_set_style(autonBlueBtn, LV_BTN_STYLE_TGL_REL, &blue_btn_tgl_rel);
    lv_btn_set_style(autonBlueBtn, LV_BTN_STYLE_REL, &blue_btn_rel);
    lv_obj_align(autonBlueBtn, controlBatt, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    // Label Red and Blue buttons
    lv_obj_t * redLabel = lv_label_create(autonRedBtn, NULL);
    lv_label_set_text(redLabel, "RED");
    lv_label_set_align(redLabel,LV_LABEL_ALIGN_CENTER);
    lv_obj_align(redLabel, autonRedBtn, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * blueLabel = lv_label_create(autonBlueBtn, NULL);
    lv_label_set_text(blueLabel, "BLUE");
    lv_label_set_align(blueLabel,LV_LABEL_ALIGN_CENTER);
    lv_obj_align(blueLabel, autonBlueBtn, LV_ALIGN_CENTER, 0, 0);

    // Set red to toggled
    lv_btn_set_state(autonRedBtn, LV_BTN_STATE_TGL_REL);

    // Create dummy line, find lineup coords, delete line
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
    

    // Create button map for auton type
    lv_btnm_set_map(autonTypeBtnm, autonTypes);
    lv_btnm_set_toggle(autonTypeBtnm, true, 0);
    // lv_obj_align(autonTypeBtnm, autonRedBtn, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_set_x(autonTypeBtnm, btnmXMin);
    lv_obj_set_y(autonTypeBtnm, btnmY);
    lv_obj_set_size(autonTypeBtnm, btnmWidth, 50);

    // Create Screen Change Buttons
    lv_obj_t * scr1ChangeBtn = lv_btn_create(scr0, NULL);
    lv_obj_set_size(scr1ChangeBtn, 40, 40);
    lv_btn_set_action(scr1ChangeBtn, LV_BTN_ACTION_CLICK, setScr1Action);
    lv_obj_align(scr1ChangeBtn, logo, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);

    lv_obj_t * scr2ChangeBtn = lv_btn_create(scr0, scr1ChangeBtn);
    lv_btn_set_action(scr1ChangeBtn, LV_BTN_ACTION_CLICK, setScr2Action);
    lv_obj_align(scr1ChangeBtn, logo, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 10);

    // Make their labels
    lv_obj_t * scr1ChangeBtnLabel = lv_label_create(scr1ChangeBtn, NULL);
    lv_label_set_align(scr1ChangeBtnLabel, LV_LABEL_ALIGN_CENTER);
    lv_label_set_text(scr1ChangeBtnLabel, "A");
    lv_obj_align(scr1ChangeBtnLabel, scr1ChangeBtn, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * scr2ChangeBtnLabel = lv_label_create(scr2ChangeBtn, scr1ChangeBtnLabel);
    lv_label_set_text(scr2ChangeBtnLabel, "S");
    lv_obj_align(scr2ChangeBtnLabel, scr2ChangeBtn, LV_ALIGN_CENTER, 0, 0);



    // SCREEN 1 OBJECTS

    // Create screen label
    lv_obj_t * autonScreenLabel = lv_label_create(scr1, NULL);
    lv_label_set_text(autonScreenLabel,"Auton Tools");
    lv_obj_align(autonScreenLabel, scr1, LV_ALIGN_IN_TOP_LEFT, 10, 10);



    // SCREEN 2 OBJECTS

    // Create screen label
    lv_obj_t * statsScreenLabel = lv_label_create(scr2, NULL);
    lv_label_set_text(statsScreenLabel,"Robot Stats");
    lv_obj_align(statsScreenLabel, scr2, LV_ALIGN_IN_TOP_LEFT, 10, 10);

    
    while(true){
        // Set values of battery bars
        lv_bar_set_value(robotBatt, pros::battery::get_voltage());
        
        lv_bar_set_value(controlBatt, master.get_battery_level());
        
        // Ensure that one color button is always toggled
        if(lv_btn_get_state(autonBlueBtn) == 1){
            lv_btn_set_state(autonRedBtn, LV_BTN_STATE_REL);
        }
        if(lv_btn_get_state(autonRedBtn) == 1){
            lv_btn_set_state(autonBlueBtn, LV_BTN_STATE_REL);
        }

        if(lv_btn_get_state(autonBlueBtn) == 3){
            lv_btn_set_state(autonRedBtn, LV_BTN_STATE_TGL_REL);
        }
        if(lv_btn_get_state(autonRedBtn) == 3){
            lv_btn_set_state(autonBlueBtn, LV_BTN_STATE_TGL_REL);
        }

        // Store auton color
        if(lv_btn_get_state(autonBlueBtn) == 2){
            autonColor = 0;
        }
        if(lv_btn_get_state(autonRedBtn) == 2){
            autonColor = 1;
        }

        // printf("%d\n", )
        

        // for(x; x<=300; x++){
        //     lv_obj_set_x(logo, x);
        //     pros::delay(15);
        // }
        // for(x; x>=0; x--){
        //     lv_obj_set_x(logo, x);
        //     pros::delay(15);
        // }
        // if(x>=300 || x<=0){
        //     xDirection = 1-xDirection;
        //     // xRate = (rand() % 2) + 1;
        // }
        // if(y>=105 || y<=10){
        //     yDirection = 1-yDirection;
        //     // yRate = (rand() % 2) + 1;
        // }
        // if(xDirection == 1){
        //     x++;
        // }
        // else{
        //     x--;
        // }
        // if(yDirection == 1){
        //     y++;
        // }
        // else{
        //     y--;
        // }
        // lv_obj_set_x(logo, x);
        // lv_obj_set_y(logo, y);
        pros::delay(100);
    }
}