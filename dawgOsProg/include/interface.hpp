
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


// SCREEN 1 OBJECTS
const char * encoderValues = "ERROR - NO READOUTS";

//Text window for motor screen
lv_obj_t * motorData = lv_ta_create(scr2, NULL);

// static lv_res_t motorDataUpdate(lv_obj_t * btn)
// {
//     uint8_t id = lv_obj_get_free_num(btn);
//     std::string output;
//     if()
//     lv_ta_add_text(motorData, str.c_str());
//     return LV_RES_OK; /*Return OK if the button is not deleted*/
// }

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

static lv_res_t hiddenBtnAction(lv_obj_t * btn)
{
    uint8_t id = lv_obj_get_free_num(btn);
    master.rumble("..");
    return LV_RES_OK; /*Return OK if the button is not deleted*/
}

static lv_res_t autonArmAction(lv_obj_t * sw)
{
    autonArmed = !autonArmed;
    return LV_RES_OK; /*Return OK if the button is not deleted*/
}

void interfaceInit(void* param)
{
    /*Set the surent system theme*/
    lv_theme_set_current(th);

    // SCREEN 0 OBJECTS
    lv_obj_t * hiddenBtn = lv_btn_create(scr0, NULL);
    lv_obj_t * robotBatt = lv_bar_create(scr0, NULL);
    lv_obj_t * logo = lv_img_create(scr0, NULL);
    lv_obj_t * controlBatt = lv_bar_create(scr0, NULL);
    lv_obj_t * robotBattLabel = lv_label_create(robotBatt, NULL);
    lv_obj_t * controlBattLabel = lv_label_create(controlBatt, NULL);
    lv_obj_t * autonTypeBtnm = lv_btnm_create(scr0, NULL);
    
    
    // Loads main screen
    lv_scr_load(scr0);
    // Create Logo and put in top left corner of main screen
    lv_img_set_src(logo, &DogLogo);
    lv_page_glue_obj(logo, true);
    lv_obj_align(logo, scr0, LV_ALIGN_IN_TOP_LEFT,5,5);
    lv_obj_set_protect(logo, LV_PROTECT_POS);
    lv_obj_set_click(logo, false);

    static lv_style_t normal_logo;
    static lv_style_t disabled_logo;
    static lv_style_t connected_logo;
    static lv_style_t autonomous_logo;

    lv_style_copy(&normal_logo, &lv_style_plain);
    lv_style_copy(&disabled_logo, &lv_style_plain);
    lv_style_copy(&connected_logo, &lv_style_plain);
    lv_style_copy(&autonomous_logo, &lv_style_plain);

    disabled_logo.image.color = LV_COLOR_RED;
    connected_logo.image.color = LV_COLOR_LIME;
    autonomous_logo.image.color = LV_COLOR_PURPLE;
    disabled_logo.image.intense = LV_OPA_50;
    connected_logo.image.intense = LV_OPA_50;
    autonomous_logo.image.intense = LV_OPA_50;


    static lv_style_t hidden_btn_pr;
    static lv_style_t hidden_btn_rel;
    hidden_btn_pr.body.opa = 0;
    hidden_btn_rel.body.opa = 0;

    lv_obj_set_size(hiddenBtn, 100, 85);
    // lv_obj_set_parent(hiddenBtn, logo);
    lv_obj_align(hiddenBtn, logo, LV_ALIGN_CENTER, 10, 10);
    lv_btn_set_style(hiddenBtn, LV_BTN_STYLE_PR, &hidden_btn_pr);
    lv_btn_set_style(hiddenBtn, LV_BTN_STYLE_REL, &hidden_btn_rel);
    lv_btn_set_action(hiddenBtn, LV_BTN_ACTION_LONG_PR, hiddenBtnAction);
    
    //Create bars for robot and controller battery and set it

    int controlCapacity = 4200;
    
    lv_obj_set_size(robotBatt, 140,30);
    lv_bar_set_range(robotBatt, 0, 100);
    lv_bar_set_value(robotBatt, pros::battery::get_capacity());

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
    int btnmY = lv_obj_get_y(line1) + 15;
    lv_obj_del(line1);
    
    // Create style for btnm pressing
    static lv_style_t style_btnm_tgl_rel;
    lv_style_copy(&style_btnm_tgl_rel, &lv_style_btn_tgl_rel);
    style_btnm_tgl_rel.body.main_color = LV_COLOR_MAKE(0xe5, 0xd2, 0x00);
    style_btnm_tgl_rel.body.grad_color = LV_COLOR_MAKE(0x9b, 0x9e, 0x00);
    

    // Create button map for auton type
    
    // int SIZE = sizeof(autonTypes)/4;
    // const char * autonTypesLoc[SIZE];
    // for(unsigned int i = 0; i < SIZE; i++){
    //     autonTypesLoc[i] = autonTypes[i];
    // }
    const char * autonTypesLoc[] = {"5", "6S", "6B", "8?", "T", "1", "90", "S", ""};
    lv_btnm_set_map(autonTypeBtnm, autonTypesLoc);
    lv_btnm_set_toggle(autonTypeBtnm, true, 0);
    // lv_obj_align(autonTypeBtnm, autonRedBtn, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_set_x(autonTypeBtnm, btnmXMin);
    lv_obj_set_y(autonTypeBtnm, btnmY);
    lv_obj_set_size(autonTypeBtnm, btnmWidth, 50);
    lv_btnm_set_style(autonTypeBtnm, LV_BTNM_STYLE_BTN_TGL_REL, &style_btnm_tgl_rel);
    
    
    lv_obj_set_x(logo, lv_obj_get_x(logo) + 10);
    
    lv_obj_set_y(logo, lv_obj_get_y(logo) - 10);

    // Create Screen Change Buttons
    lv_obj_t * scr1ChangeBtn = lv_btn_create(scr0, NULL);
    lv_obj_set_size(scr1ChangeBtn, 40, 40);
    lv_btn_set_action(scr1ChangeBtn, LV_BTN_ACTION_CLICK, setScr1Action);
    lv_obj_align(scr1ChangeBtn, logo, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

    lv_obj_t * scr2ChangeBtn = lv_btn_create(scr0, scr1ChangeBtn);
    lv_btn_set_action(scr2ChangeBtn, LV_BTN_ACTION_CLICK, setScr2Action);
    lv_obj_align(scr2ChangeBtn, logo, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    // Make their labels
    lv_obj_t * scr1ChangeBtnLabel = lv_label_create(scr1ChangeBtn, NULL);
    lv_label_set_align(scr1ChangeBtnLabel, LV_LABEL_ALIGN_CENTER);
    lv_label_set_text(scr1ChangeBtnLabel, "A");
    lv_obj_align(scr1ChangeBtnLabel, scr1ChangeBtn, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * scr2ChangeBtnLabel = lv_label_create(scr2ChangeBtn, scr1ChangeBtnLabel);
    lv_label_set_text(scr2ChangeBtnLabel, "M");
    lv_obj_align(scr2ChangeBtnLabel, scr2ChangeBtn, LV_ALIGN_CENTER, 0, 0);

    // lv_obj_set_y(logo, lv_obj_get_y(logo) - 2);

    lv_obj_t * autonArmedLED = lv_led_create(scr0, NULL);
    lv_obj_set_size(autonArmedLED, lv_obj_get_width(autonArmedLED)/2, lv_obj_get_width(autonArmedLED)/2);
    lv_obj_align(autonArmedLED, autonBlueBtn, LV_ALIGN_OUT_LEFT_MID, 0, 0);
    lv_obj_set_x(autonArmedLED, 285);
    



    // SCREEN 1 OBJECTS

    // Create screen label
    lv_obj_t * autonScreenLabel = lv_label_create(scr1, NULL);
    lv_label_set_text(autonScreenLabel,"Auton Tools");
    lv_obj_align(autonScreenLabel, scr1, LV_ALIGN_IN_TOP_LEFT, 20, 10);

    lv_obj_t * homeBtn1 = lv_btn_create(scr1, scr1ChangeBtn);
    lv_btn_set_action(homeBtn1, LV_BTN_ACTION_CLICK, setScr0Action);
    lv_obj_set_pos(homeBtn1, lv_obj_get_x(scr1ChangeBtn), lv_obj_get_y(scr1ChangeBtn));
    // lv_obj_align(homeBtn1, scr1, LV_ALIGN_IN_BOTTOM_LEFT, 20, -20);

    lv_obj_t * homeBtn1Label = lv_label_create(homeBtn1, scr1ChangeBtnLabel);
    lv_label_set_text(homeBtn1Label, "H");
    lv_obj_align(homeBtn1Label, homeBtn1, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * autonArmSw = lv_sw_create(scr1, NULL);
    lv_obj_align(autonArmSw, NULL, LV_ALIGN_IN_TOP_RIGHT, -10, 10);
    lv_sw_set_action(autonArmSw, autonArmAction);

    lv_obj_t * autonArmSwLabel = lv_label_create(scr1, NULL);
    lv_label_set_text(autonArmSwLabel, "Arm Auton (A)");
    lv_obj_align(autonArmSwLabel, autonArmSw, LV_ALIGN_OUT_LEFT_MID, -10, 0);

    lv_obj_t * debugData = lv_ta_create(scr1, NULL);
    lv_obj_set_size(debugData, LV_HOR_RES/2, 145);
    lv_obj_align(debugData, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -10, -20);
    lv_ta_set_cursor_type(debugData, LV_CURSOR_HIDDEN);

    lv_obj_t * LEDTypeBtnm = lv_btnm_create(scr1, NULL);
    const char * LEDTypesLoc[] = {"A", "X", "L", "H", "F", "P", "S", ""};
    lv_btnm_set_map(LEDTypeBtnm, LEDTypesLoc);
    lv_btnm_set_toggle(LEDTypeBtnm, true, 0);
    // lv_obj_align(autonTypeBtnm, autonRedBtn, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_align(LEDTypeBtnm, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -30, -50);
    lv_obj_set_size(LEDTypeBtnm, btnmWidth, 50);
    lv_btnm_set_style(LEDTypeBtnm, LV_BTNM_STYLE_BTN_TGL_REL, &style_btnm_tgl_rel);


    // SCREEN 2 OBJECTS

    // Create screen label
    lv_obj_t * motorScreenLabel = lv_label_create(scr2, NULL);
    lv_label_set_text(motorScreenLabel,"Motor Stats");
    lv_obj_align(motorScreenLabel, scr2, LV_ALIGN_IN_TOP_LEFT, 20, 10);

    lv_obj_t * homeBtn2 = lv_btn_create(scr2, homeBtn1);
    // lv_obj_align(homeBtn2, scr2, LV_ALIGN_IN_BOTTOM_LEFT, 20, -20);
    lv_obj_set_pos(homeBtn1, lv_obj_get_x(scr1ChangeBtn), lv_obj_get_y(scr1ChangeBtn));

    lv_obj_t * homeBtn2Label = lv_label_create(homeBtn2, homeBtn1Label);
    lv_obj_align(homeBtn2Label, homeBtn2, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * motorTempMeter1Btn = lv_btn_create(scr2, NULL);
    lv_obj_t * motorTempMeter2Btn = lv_btn_create(motorTempMeter1Btn, NULL);
    lv_obj_t * motorTempMeter3Btn = lv_btn_create(motorTempMeter1Btn, NULL);
    lv_obj_t * motorTempMeter4Btn = lv_btn_create(motorTempMeter1Btn, NULL);
    lv_obj_t * motorTempMeter5Btn = lv_btn_create(motorTempMeter1Btn, NULL);
    lv_obj_t * motorTempMeter6Btn = lv_btn_create(motorTempMeter1Btn, NULL);
    lv_obj_t * motorTempMeter7Btn = lv_btn_create(motorTempMeter1Btn, NULL);
    lv_obj_t * motorTempMeter8Btn = lv_btn_create(motorTempMeter1Btn, NULL);

    lv_obj_set_size(motorTempMeter1Btn, 40, 35);
    lv_obj_set_size(motorTempMeter2Btn, 40, 35);
    lv_obj_set_size(motorTempMeter3Btn, 40, 35);
    lv_obj_set_size(motorTempMeter4Btn, 40, 35);
    lv_obj_set_size(motorTempMeter5Btn, 40, 35);
    lv_obj_set_size(motorTempMeter6Btn, 40, 35);
    lv_obj_set_size(motorTempMeter7Btn, 40, 35);
    lv_obj_set_size(motorTempMeter8Btn, 40, 35);
    
    static lv_style_t style_lmeter1;
    lv_style_copy(&style_lmeter1, &lv_style_pretty_color);
    style_lmeter1.line.width = 2;
    style_lmeter1.line.color = LV_COLOR_SILVER;
    style_lmeter1.body.main_color = LV_COLOR_HEX(0xc3ff00);
    style_lmeter1.body.grad_color = LV_COLOR_HEX(0xff0000);

    lv_obj_t * motorTempMeter1 = lv_lmeter_create(scr2, NULL);
    lv_lmeter_set_style(motorTempMeter1, &style_lmeter1);
    lv_lmeter_set_scale(motorTempMeter1, 240, 200);
    lv_obj_set_size(motorTempMeter1, lv_obj_get_width(motorTempMeter1)/1.85, lv_obj_get_height(motorTempMeter1)/1.85);
    lv_lmeter_set_range(motorTempMeter1, 25, 60);
    lv_lmeter_set_value(motorTempMeter1, 50);
    lv_obj_align(motorTempMeter1, scr2, LV_ALIGN_IN_TOP_RIGHT, -30, 25);

    lv_obj_t * motorTempMeter2 = lv_lmeter_create(scr2, motorTempMeter1);
    lv_lmeter_set_value(motorTempMeter2, 50);
    lv_obj_align(motorTempMeter2, motorTempMeter1, LV_ALIGN_OUT_BOTTOM_MID, 0, -7);

    lv_obj_t * motorTempMeter3 = lv_lmeter_create(scr2, motorTempMeter1);
    lv_lmeter_set_value(motorTempMeter3, 50);
    lv_obj_align(motorTempMeter3, motorTempMeter2, LV_ALIGN_OUT_BOTTOM_MID, 0, -7);

    lv_obj_t * motorTempMeter4 = lv_lmeter_create(scr2, motorTempMeter1);
    lv_lmeter_set_value(motorTempMeter4, 50);
    lv_obj_align(motorTempMeter4, motorTempMeter1, LV_ALIGN_OUT_LEFT_MID, -20, 0);

    lv_obj_t * motorTempMeter5 = lv_lmeter_create(scr2, motorTempMeter1);
    lv_lmeter_set_value(motorTempMeter5, 50);
    lv_obj_align(motorTempMeter5, motorTempMeter4, LV_ALIGN_OUT_BOTTOM_MID, 0, -7);

    lv_obj_t * motorTempMeter6 = lv_lmeter_create(scr2, motorTempMeter1);
    lv_lmeter_set_value(motorTempMeter6, 50);
    lv_obj_align(motorTempMeter6, motorTempMeter5, LV_ALIGN_OUT_BOTTOM_MID, 0, -7);

    lv_obj_t * motorTempMeter7 = lv_lmeter_create(scr2, motorTempMeter1);
    lv_lmeter_set_value(motorTempMeter7, 50);
    lv_obj_align(motorTempMeter7, motorTempMeter4, LV_ALIGN_OUT_LEFT_MID, -20, 0);

    lv_obj_t * motorTempMeter8 = lv_lmeter_create(scr2, motorTempMeter1);
    lv_lmeter_set_value(motorTempMeter8, 50);
    lv_obj_align(motorTempMeter8, motorTempMeter7, LV_ALIGN_OUT_BOTTOM_MID, 0, -7);
    
    lv_obj_set_parent(motorTempMeter1Btn, motorTempMeter1);
    lv_obj_set_parent(motorTempMeter2Btn, motorTempMeter2);
    lv_obj_set_parent(motorTempMeter3Btn, motorTempMeter3);
    lv_obj_set_parent(motorTempMeter4Btn, motorTempMeter4);
    lv_obj_set_parent(motorTempMeter5Btn, motorTempMeter5);
    lv_obj_set_parent(motorTempMeter6Btn, motorTempMeter6);
    lv_obj_set_parent(motorTempMeter7Btn, motorTempMeter7);
    lv_obj_set_parent(motorTempMeter8Btn, motorTempMeter8);

    // lv_btn_set_action(motorTempMeter1Btn, LV_BTN_ACTION_CLICK, motorDataUpdate);
    // lv_btn_set_action(motorTempMeter2Btn, LV_BTN_ACTION_CLICK, motorDataUpdate);
    // lv_btn_set_action(motorTempMeter3Btn, LV_BTN_ACTION_CLICK, motorDataUpdate);
    // lv_btn_set_action(motorTempMeter4Btn, LV_BTN_ACTION_CLICK, motorDataUpdate);
    // lv_btn_set_action(motorTempMeter5Btn, LV_BTN_ACTION_CLICK, motorDataUpdate);
    // lv_btn_set_action(motorTempMeter6Btn, LV_BTN_ACTION_CLICK, motorDataUpdate);
    // lv_btn_set_action(motorTempMeter7Btn, LV_BTN_ACTION_CLICK, motorDataUpdate);
    // lv_btn_set_action(motorTempMeter8Btn, LV_BTN_ACTION_CLICK, motorDataUpdate);

    lv_obj_align(motorTempMeter1Btn, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(motorTempMeter2Btn, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(motorTempMeter3Btn, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(motorTempMeter4Btn, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(motorTempMeter5Btn, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(motorTempMeter6Btn, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(motorTempMeter7Btn, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(motorTempMeter8Btn, NULL, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * motorTempMeter1Label = lv_label_create(motorTempMeter1Btn, NULL);
    lv_obj_t * motorTempMeter2Label = lv_label_create(motorTempMeter2Btn, NULL);
    lv_obj_t * motorTempMeter3Label = lv_label_create(motorTempMeter3Btn, NULL);
    lv_obj_t * motorTempMeter4Label = lv_label_create(motorTempMeter4Btn, NULL);
    lv_obj_t * motorTempMeter5Label = lv_label_create(motorTempMeter5Btn, NULL);
    lv_obj_t * motorTempMeter6Label = lv_label_create(motorTempMeter6Btn, NULL);
    lv_obj_t * motorTempMeter7Label = lv_label_create(motorTempMeter7Btn, NULL);
    lv_obj_t * motorTempMeter8Label = lv_label_create(motorTempMeter8Btn, NULL);
    
    lv_label_set_align(motorTempMeter1Label, LV_LABEL_ALIGN_CENTER);
    lv_label_set_align(motorTempMeter2Label, LV_LABEL_ALIGN_CENTER);
    lv_label_set_align(motorTempMeter3Label, LV_LABEL_ALIGN_CENTER);
    lv_label_set_align(motorTempMeter4Label, LV_LABEL_ALIGN_CENTER);
    lv_label_set_align(motorTempMeter5Label, LV_LABEL_ALIGN_CENTER);
    lv_label_set_align(motorTempMeter6Label, LV_LABEL_ALIGN_CENTER);
    lv_label_set_align(motorTempMeter7Label, LV_LABEL_ALIGN_CENTER);
    lv_label_set_align(motorTempMeter8Label, LV_LABEL_ALIGN_CENTER);

    lv_label_set_text(motorTempMeter1Label, "L");
    lv_label_set_text(motorTempMeter2Label, "IR");
    lv_label_set_text(motorTempMeter3Label, "IL");
    lv_label_set_text(motorTempMeter4Label, "FR");
    lv_label_set_text(motorTempMeter5Label, "BR");
    lv_label_set_text(motorTempMeter6Label, "T");
    lv_label_set_text(motorTempMeter7Label, "FL");
    lv_label_set_text(motorTempMeter8Label, "BL");

    lv_obj_align(motorTempMeter1Label, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(motorTempMeter2Label, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(motorTempMeter3Label, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(motorTempMeter4Label, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(motorTempMeter5Label, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(motorTempMeter6Label, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(motorTempMeter7Label, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(motorTempMeter8Label, NULL, LV_ALIGN_CENTER, 0, 0);
    
    lv_obj_set_width(motorData, lv_obj_get_width(motorData) - 70);

    static lv_style_t style_bg;
    lv_style_copy(&style_bg, lv_ta_get_style(motorData, LV_TA_STYLE_BG));
    style_bg.text.font = &lv_font_dejavu_20;
    lv_ta_set_style(motorData, LV_TA_STYLE_BG, &style_bg);
    lv_ta_set_cursor_type(motorData, LV_CURSOR_HIDDEN);
    lv_ta_set_text(motorData, "SELECT MOTOR");
    lv_obj_set_height(motorData, lv_obj_get_height(motorData)-10);

    lv_obj_t * encoderResetBtn = lv_btn_create(scr2, NULL);
    lv_obj_set_size(encoderResetBtn, 85, 60);
    lv_obj_align(encoderResetBtn, motorTempMeter8, LV_ALIGN_OUT_BOTTOM_MID, 0, -10);

    lv_obj_t * encoderResetBtnLabel = lv_label_create(encoderResetBtn, NULL);
    lv_label_set_align(encoderResetBtnLabel, LV_LABEL_ALIGN_CENTER);
    lv_label_set_text(encoderResetBtnLabel, "Reset\nEncoder");
    lv_obj_align(encoderResetBtnLabel, NULL, LV_ALIGN_CENTER, 0, 0);

    // LOOP VARS
    std::string readout;
    int currentMotorRead = 0;

    
    // DISPLAY UPDATE LOOP
    while(true){
        // HOME SCREEN ACTIONS
        if(lv_scr_act() == scr0){

            // Set logo theme based on comp status
            if(pros::competition::is_disabled()){
                lv_obj_set_style(logo, &disabled_logo);
            }
            else if(pros::competition::is_autonomous()){
                lv_obj_set_style(logo, &autonomous_logo);
            }
            else if(pros::competition::is_connected()){
                lv_obj_set_style(logo, &connected_logo);
            }
            else{
                lv_obj_set_style(logo, &normal_logo);
            }
            
            // Set values of battery bars
            lv_bar_set_value(robotBatt, pros::battery::get_capacity());
            
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
                autonColor = -1;
            }
            if(lv_btn_get_state(autonRedBtn) == 2){
                autonColor = 1;
            }

            if(autonArmed){
                lv_led_set_bright(autonArmedLED, 255);
            }
            else{
                lv_led_set_bright(autonArmedLED, 0);
            }

            autonType = lv_btnm_get_toggled(autonTypeBtnm);
        }

        else if (lv_scr_act() == scr1){
            lv_ta_set_text(debugData, debugOutput.c_str());
            lv_ta_set_cursor_pos(debugData, 0);
            selectedLED = lv_btnm_get_toggled(LEDTypeBtnm);
        }
        // MOTOR SCREEN ACTIONS
        else if (lv_scr_act() == scr2){
            
            lv_lmeter_set_value(motorTempMeter1, lift.get_temperature());
            lv_lmeter_set_value(motorTempMeter2, intakeR.get_temperature());
            lv_lmeter_set_value(motorTempMeter3, intakeL.get_temperature());
            lv_lmeter_set_value(motorTempMeter4, driveFR.get_temperature());
            lv_lmeter_set_value(motorTempMeter5, driveBR.get_temperature());
            lv_lmeter_set_value(motorTempMeter6, tray.get_temperature());
            lv_lmeter_set_value(motorTempMeter7, driveFL.get_temperature());
            lv_lmeter_set_value(motorTempMeter8, driveBL.get_temperature());

            // lv_ta_set_text(motorData, "loop working");
            // std::string readout = std::to_string((int) driveBL.get_temperature()) + " this works";
            if(lv_btn_get_state(motorTempMeter1Btn) == 1){
                currentMotorRead = 1;
            }
            else if(lv_btn_get_state(motorTempMeter2Btn) == 1){
                currentMotorRead = 2;
            }
            else if(lv_btn_get_state(motorTempMeter3Btn) == 1){
                currentMotorRead = 3;
            }
            else if(lv_btn_get_state(motorTempMeter4Btn) == 1){
                currentMotorRead = 4;
            }
            else if(lv_btn_get_state(motorTempMeter5Btn) == 1){
                currentMotorRead = 5;
            }
            else if(lv_btn_get_state(motorTempMeter6Btn) == 1){
                currentMotorRead = 6;
            }
            else if(lv_btn_get_state(motorTempMeter7Btn) == 1){
                currentMotorRead = 7;
            }
            else if(lv_btn_get_state(motorTempMeter8Btn) == 1){
                currentMotorRead = 8;
            }

            switch (currentMotorRead){
                case 1: readout = "Lift         (P" + std::to_string(lift_port) + ")\nTemp: " + std::to_string((int) lift.get_temperature()) + "\nEncoder: " + std::to_string((int) lift.get_position()) + "\nVelocity: " + std::to_string(lift.get_actual_velocity());
                break;

                case 2: readout = "Intake Right (P" + std::to_string(intakeR_port) + ")\nTemp: " + std::to_string((int) intakeR.get_temperature()) + "\nEncoder: " + std::to_string((int) intakeR.get_position()) + "\nVelocity: " + std::to_string(intakeR.get_actual_velocity());
                        break;

                case 3: readout = "Intake Left  (P" + std::to_string(intakeL_port) + ")\nTemp: " + std::to_string((int) intakeL.get_temperature()) + "\nEncoder: " + std::to_string((int) intakeL.get_position()) + "\nVelocity: " + std::to_string(intakeL.get_actual_velocity());
                        break;
                // Correct \/
                case 4: readout = "Front Right  (P" + std::to_string(driveFR_port) + ")\nTemp: " + std::to_string((int) driveFR.get_temperature()) + "\nEncoder: " + std::to_string((int) driveFR.get_position()) + "\nVelocity: " + std::to_string(driveFR.get_actual_velocity());
                        break;

                case 5: readout = "Back Right   (P" + std::to_string(driveBR_port) + ")\nTemp: " + std::to_string((int) driveBR.get_temperature()) + "\nEncoder: " + std::to_string((int) driveBR.get_position()) + "\nVelocity: " + std::to_string(driveBR.get_actual_velocity());
                        break;

                case 6: readout = "Tray         (P" + std::to_string(tray_port) + ")\nTemp: " + std::to_string((int) tray.get_temperature()) + "\nEncoder: " + std::to_string((int) tray.get_position()) + "\nVelocity: " + std::to_string(tray.get_actual_velocity());
                        break;

                case 7: readout = "Front Left   (P" + std::to_string(driveFL_port) + ")\nTemp: " + std::to_string((int) driveFL.get_temperature()) + "\nEncoder: " + std::to_string((int) driveFL.get_position()) + "\nVelocity: " + std::to_string(driveFL.get_actual_velocity());
                        break;

                case 8: readout = "Back Left    (P" + std::to_string(driveBL_port) + ")\nTemp: " + std::to_string((int) driveBL.get_temperature()) + "\nEncoder: " + std::to_string((int) driveBL.get_position()) + "\nVelocity: " + std::to_string(driveBL.get_actual_velocity());
                        break;

                default: readout = "Please Select Motor....";
                        break;
            }

            if(lv_btn_get_state(encoderResetBtn) == 1){
                switch (currentMotorRead){
                    case 1: lift.tare_position();
                            break;

                    case 2: intakeR.tare_position();
                            break;

                    case 3: intakeL.tare_position();
                            break;
                    // Correct \/
                    case 4: driveFR.tare_position();
                            break;

                    case 5: driveBR.tare_position();
                            break;

                    case 6: tray.tare_position();
                            break;

                    case 7: driveFL.tare_position();
                            break;

                    case 8: driveBL.tare_position();
                            break;

                    default: break;
                }
            }
            
            readout.erase (readout.end()-4, readout.end());
            lv_ta_set_text(motorData, readout.c_str());
            lv_ta_set_cursor_pos(motorData, 0);
        }


        pros::delay(100);
    }
}