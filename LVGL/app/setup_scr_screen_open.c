/*
 * Copyright 2026 NXP
 * SPDX-License-Identifier: MIT
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"

void setup_scr_screen_open(lv_ui *ui)
{

	// Write codes screen_open
	ui->screen_open = lv_obj_create(NULL, NULL);

	// Write style LV_OBJ_PART_MAIN for screen_open
	static lv_style_t style_screen_open_main;
	lv_style_reset(&style_screen_open_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_open_main
	lv_style_set_bg_color(&style_screen_open_main, LV_STATE_DEFAULT, lv_color_make(0x3c, 0xdd, 0xc2));
	lv_style_set_bg_opa(&style_screen_open_main, LV_STATE_DEFAULT, 126);
	lv_obj_add_style(ui->screen_open, LV_OBJ_PART_MAIN, &style_screen_open_main);

	// Write codes screen_open_label_huanying
	ui->screen_open_label_huanying = lv_label_create(ui->screen_open, NULL);
	lv_label_set_text(ui->screen_open_label_huanying, "欢迎使用空气质量检测仪 3.3V");
	lv_label_set_long_mode(ui->screen_open_label_huanying, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_open_label_huanying, LV_LABEL_ALIGN_CENTER);

	// Write style LV_LABEL_PART_MAIN for screen_open_label_huanying
	static lv_style_t style_screen_open_label_huanying_main;
	lv_style_reset(&style_screen_open_label_huanying_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_open_label_huanying_main
	lv_style_set_radius(&style_screen_open_label_huanying_main, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_color(&style_screen_open_label_huanying_main, LV_STATE_DEFAULT, lv_color_make(0x29, 0xae, 0x32));
	lv_style_set_bg_grad_color(&style_screen_open_label_huanying_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_open_label_huanying_main, LV_STATE_DEFAULT, LV_GRAD_DIR_HOR);
	lv_style_set_bg_opa(&style_screen_open_label_huanying_main, LV_STATE_DEFAULT, 255);
	lv_style_set_text_color(&style_screen_open_label_huanying_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_open_label_huanying_main, LV_STATE_DEFAULT, &lv_font_SIMLI_25);
	lv_style_set_text_letter_space(&style_screen_open_label_huanying_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_open_label_huanying_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_open_label_huanying_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_open_label_huanying_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_open_label_huanying_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_open_label_huanying, LV_LABEL_PART_MAIN, &style_screen_open_label_huanying_main);
	lv_obj_set_pos(ui->screen_open_label_huanying, 20, 20);
	lv_obj_set_size(ui->screen_open_label_huanying, 200, 0);

	// Write codes screen_open_label_ap
	ui->screen_open_label_ap = lv_label_create(ui->screen_open, NULL);
	lv_label_set_text(ui->screen_open_label_ap, "点击按键四进入配网模式 ");
	lv_label_set_long_mode(ui->screen_open_label_ap, LV_LABEL_LONG_BREAK);
	lv_label_set_align(ui->screen_open_label_ap, LV_LABEL_ALIGN_LEFT);

	// Write style LV_LABEL_PART_MAIN for screen_open_label_ap
	static lv_style_t style_screen_open_label_ap_main;
	lv_style_reset(&style_screen_open_label_ap_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_open_label_ap_main
	lv_style_set_radius(&style_screen_open_label_ap_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_screen_open_label_ap_main, LV_STATE_DEFAULT, lv_color_make(0x29, 0xae, 0x32));
	lv_style_set_bg_grad_color(&style_screen_open_label_ap_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_open_label_ap_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_open_label_ap_main, LV_STATE_DEFAULT, 255);
	lv_style_set_text_color(&style_screen_open_label_ap_main, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_font(&style_screen_open_label_ap_main, LV_STATE_DEFAULT, &font16);
	lv_style_set_text_letter_space(&style_screen_open_label_ap_main, LV_STATE_DEFAULT, 2);
	lv_style_set_pad_left(&style_screen_open_label_ap_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_open_label_ap_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_open_label_ap_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_open_label_ap_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_open_label_ap, LV_LABEL_PART_MAIN, &style_screen_open_label_ap_main);
	lv_obj_set_pos(ui->screen_open_label_ap, 15, 260);
	lv_obj_set_size(ui->screen_open_label_ap, 210, 0);

	// Write codes screen_open_img_zhutou
	ui->screen_open_img_zhutou = lv_img_create(ui->screen_open, NULL);

	// Write style LV_IMG_PART_MAIN for screen_open_img_zhutou
	static lv_style_t style_screen_open_img_zhutou_main;
	lv_style_reset(&style_screen_open_img_zhutou_main);

	// Write style state: LV_STATE_DEFAULT for style_screen_open_img_zhutou_main
	lv_style_set_image_recolor(&style_screen_open_img_zhutou_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_screen_open_img_zhutou_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_screen_open_img_zhutou_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_open_img_zhutou, LV_IMG_PART_MAIN, &style_screen_open_img_zhutou_main);
	lv_obj_set_pos(ui->screen_open_img_zhutou, 70, 120);
	lv_obj_set_size(ui->screen_open_img_zhutou, 100, 100);
	lv_obj_set_click(ui->screen_open_img_zhutou, true);
	lv_img_set_src(ui->screen_open_img_zhutou, &_zhutou_alpha_100x100);
	lv_img_set_pivot(ui->screen_open_img_zhutou, 0, 0);
	lv_img_set_angle(ui->screen_open_img_zhutou, 0);
}