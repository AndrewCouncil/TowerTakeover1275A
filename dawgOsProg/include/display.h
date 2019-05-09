
#include "display/lv_themes/lv_theme_templ_code.h"
#include "display/lv_themes/lv_theme_default_code.h"
// #include "display/lv_themes/lv_theme_alien_code.h"
#include "display/lv_themes/lv_theme_night_code.h"
#include "display/lv_themes/lv_theme_mono_code.h"



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
    // extern const lv_img_t DogLogo;
    
    //pros::Motor test(15);
    /********************
     * CREATE A SCREEN
     *******************/
    /* Create a new screen and load it
     * Screen can be created from any type object type
     * Now a Page is used which is an objects with scrollable content*/

    /*Initialize the alien theme
     * 210: a green HUE value
     * NULL: use the default font (LV_FONT_DEFAULT)*/
    lv_theme_t * th = lv_theme_night_init(65, NULL);
    
    /*Set the surent system theme*/
    lv_theme_set_current(th);


    lv_obj_t * scr = lv_page_create(NULL, NULL);
    lv_scr_load(scr);
    
    lv_obj_t * logo = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(logo, &DogLogo);

    /****************
     * ADD A TITLE
     ****************/
    lv_obj_t * label = lv_label_create(scr, NULL); /*First parameters (scr) is the parent*/
    lv_label_set_text(label, "Object usage demo");  /*Set the text*/
    lv_obj_set_x(label, 50);                        /*Set the x coordinate*/

    /***********************
     * CREATE TWO BUTTONS
     ***********************/
    /*Create a button*/
    lv_obj_t * btn1 = lv_btn_create(lv_scr_act(), NULL);         /*Create a button on the currently loaded screen*/
    // lv_btn_set_action(btn1, LV_BTN_ACTION_CLICK, btn_rel_action); /*Set function to be called when the button is released*/
    lv_obj_align(btn1, label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);  /*Align below the label*/
    lv_btn_set_toggle(btn1, true);
    /*Create a label on the button (the 'label' variable can be reused)*/
    label = lv_label_create(btn1, NULL);
    lv_label_set_text(label, "Button 1");
    
    /*Copy the previous button*/
    lv_obj_t * btn2 = lv_btn_create(lv_scr_act(), btn1);        /*Second parameter is an object to copy*/
    lv_btn_set_toggle(btn2, false);
    lv_obj_align(btn2, btn1, LV_ALIGN_OUT_RIGHT_MID, 50, 0);    /*Align next to the prev. button.*/

    /*Create a label on the button*/
    label = lv_label_create(btn2, NULL);
    lv_label_set_text(label, "Button 2");

    /****************
     * ADD A SLIDER
     ****************/
    lv_obj_t * slider = lv_slider_create(scr, NULL);                            /*Create a slider*/
    lv_obj_set_size(slider, lv_obj_get_width(lv_scr_act())  / 3, LV_DPI / 3);   /*Set the size*/
    lv_obj_align(slider, btn1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);                /*Align below the first button*/
    lv_slider_set_value(slider, 30);                                            /*Set the current value*/

    /***********************
     * ADD A DROP DOWN LIST
     ************************/
    lv_obj_t * ddlist = lv_ddlist_create(lv_scr_act(), NULL);            /*Create a drop down list*/
    lv_obj_align(ddlist, slider, LV_ALIGN_OUT_RIGHT_TOP, 50, 0);         /*Align next to the slider*/
    lv_obj_set_free_ptr(ddlist, slider);                                   /*Save the pointer of the slider in the ddlist (used in 'ddlist_action()')*/
    lv_obj_set_top(ddlist, true);                                        /*Enable to be on the top when clicked*/
    lv_ddlist_set_options(ddlist, "None\nLittle\nHalf\nA lot\nAll"); /*Set the options*/
    // lv_ddlist_set_action(ddlist, ddlist_action);                         /*Set function to call on new option is chosen*/

    /****************
     * CREATE A CHART
     ****************/
    lv_obj_t * chart = lv_chart_create(lv_scr_act(), NULL);                         /*Create the chart*/
    lv_obj_set_size(chart, lv_obj_get_width(scr) / 2, lv_obj_get_width(scr) / 4);   /*Set the size*/
    lv_obj_align(chart, slider, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 50);                   /*Align below the slider*/
    lv_chart_set_series_width(chart, 3);                                            /*Set the line width*/

    /*Add a RED data series and set some points*/
    lv_chart_series_t * dl1 = lv_chart_add_series(chart, LV_COLOR_RED);
    lv_chart_set_next(chart, dl1, 10);
    lv_chart_set_next(chart, dl1, 25);
    lv_chart_set_next(chart, dl1, 45);
    lv_chart_set_next(chart, dl1, 80);

    /*Add a BLUE data series and set some points*/
    lv_chart_series_t * dl2 = lv_chart_add_series(chart, LV_COLOR_MAKE(0x40, 0x70, 0xC0));
    lv_chart_set_next(chart, dl2, 10);
    lv_chart_set_next(chart, dl2, 25);
    lv_chart_set_next(chart, dl2, 45);
    lv_chart_set_next(chart, dl2, 80);
    lv_chart_set_next(chart, dl2, 75);
    lv_chart_set_next(chart, dl2, 505);

    lv_obj_t * status = lv_label_create(scr, NULL); /*First parameters (scr) is the parent*/
    lv_obj_set_x(label, 150);

    
    lv_obj_set_x(logo, 150);
    lv_obj_set_y(logo, 50);
    int x = 1;
    int y = 50;
    int xDirection = 1;
    int yDirection = 1;
    int xRate = 1;
    int yRate = 1;
    while(true){
        // for(x; x<=300; x++){
        //     lv_obj_set_x(logo, x);
        //     pros::delay(15);
        // }
        // for(x; x>=0; x--){
        //     lv_obj_set_x(logo, x);
        //     pros::delay(15);
        // }
        if(x>=300 || x<=0){
            xDirection = 1-xDirection;
            // xRate = (rand() % 2) + 1;
        }
        if(y>=105 || y<=10){
            yDirection = 1-yDirection;
            // yRate = (rand() % 2) + 1;
        }
        if(xDirection == 1){
            x++;
        }
        else{
            x--;
        }
        if(yDirection == 1){
            y++;
        }
        else{
            y--;
        }
        lv_obj_set_x(logo, x);
        lv_obj_set_y(logo, y);
        pros::delay(15);
    }
}