#include "Config_parser.h"

int filledDsplByLines = 0;
VMUINT8 *buffer;
VMCHAR myHost[100] = {};
VMCHAR myPort[100] = {};  //maybe affter need convert to int
VMCHAR myLogin[100] = {};
VMCHAR myPassword[100] = {};
VMCHAR myCommand[100] = {};
VMINT trigeris = 0;
VMWCHAR file_pathw[100];

void vm_main(void) {

    layer_hdl[0] = -1;
    vm_reg_sysevt_callback(handle_sysevt);
    vm_reg_keyboard_callback(handle_keyevt);
    vm_font_set_font_size(VM_SMALL_FONT);
    checkFileExist();

}

void handle_sysevt(VMINT message, VMINT param) {

    switch (message) {
        case VM_MSG_CREATE:
        case VM_MSG_ACTIVE:
            layer_hdl[0] =
                vm_graphic_create_layer(0, 0, vm_graphic_get_screen_width(),
                                        vm_graphic_get_screen_height(), -1);
            vm_graphic_set_clip(0, 0, vm_graphic_get_screen_width(),
                                vm_graphic_get_screen_height());
            buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);
            vm_switch_power_saving_mode(turn_off_mode);
            break;

        case VM_MSG_PAINT:
            vm_switch_power_saving_mode(turn_off_mode);
            showResult();
            break;

        case VM_MSG_INACTIVE:
            vm_switch_power_saving_mode(turn_on_mode);
            if (layer_hdl[0] != -1) vm_graphic_delete_layer(layer_hdl[0]);
            break;

        case VM_MSG_QUIT:
            if (layer_hdl[0] != -1) vm_graphic_delete_layer(layer_hdl[0]);
            break;
    }
}

void handle_keyevt(VMINT event, VMINT keycode) {

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_RIGHT_SOFTKEY) {
        if (layer_hdl[0] != -1) {
            vm_graphic_delete_layer(layer_hdl[0]);
            layer_hdl[0] = -1;
        }
        vm_exit_app();
    }

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_LEFT_SOFTKEY) {
            showResult();
    }

}

void mre_draw_black_rectangle(void) {

    buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);
    vm_graphic_fill_rect(buffer, 0, 0, vm_graphic_get_screen_width(), vm_graphic_get_screen_height(), VM_COLOR_BLACK, VM_COLOR_BLACK);
    vm_graphic_flush_layer(layer_hdl, 1);
    filledDsplByLines = 0;
}

void vertical_scrolling_ucs2_text(VMWSTR ucs2_string) {
    int max_height;
    int font_height = 16;
    int i;
    int abc = 0;

    if (filledDsplByLines == 0) {
       abc = 2;
       mre_draw_black_rectangle();
    }

    max_height = vm_graphic_get_screen_height();
    vm_graphic_clear_layer_bg(layer_hdl[0]);
    vm_graphic_textout_by_baseline(buffer, 0, filledDsplByLines + abc, ucs2_string, vm_wstrlen(ucs2_string), VM_COLOR_WHITE, 12);
    vm_graphic_flush_layer(layer_hdl, 1);
    filledDsplByLines += font_height;
}

void create_app_txt_path(VMWSTR text) {

    VMWCHAR fullPath[100];
    //VMWCHAR appName[100];
    //VMWCHAR path[100];
    VMWCHAR wfile_extension[4];

    vm_get_exec_filename(fullPath);
    //vm_get_path(fullPath, path); //if need same directory as vxp
    //vm_get_filename(fullPath, appName);
    //vm_wstrcat(path, appName);
    vm_ascii_to_ucs2(wfile_extension, 8, "txt");
    vm_wstrncpy(text, fullPath, vm_wstrlen(fullPath) - 3);
    vm_wstrcat(text, wfile_extension);

}

void checkFileExist(void) {

    VMFILE f_read;
    VMUINT nread;
    //VMWCHAR file_pathw[100];
    VMCHAR new_data[500];

    create_app_txt_path(file_pathw);

    f_read = vm_file_open(file_pathw, MODE_READ, FALSE);

    if (f_read < 0) {
       vm_file_close(f_read);
       trigeris = 1;
    } else {
      vm_file_read(f_read, new_data, 500, &nread);
      new_data[nread] = '\0';
      vm_file_close(f_read);
      parseText(new_data);
    }
}

VMINT parseText(VMSTR text) {

    VMCHAR vns_simbl[2] = {};
    VMCHAR nauj_strng[100] = {};
    VMCHAR nauj_strng1[100] = {};
    VMINT counter = 0;
    VMINT counter1 = 0;
    VMCHAR *ptr;

    ptr = text;

    while (*ptr != '\0' || counter1 == 5) {

          if (*ptr == '\n') {

             //if (strlen(nauj_strng) < 1) { break; } // bad data ?

             counter = counter + 1;

             if (counter == 1) {strcpy(myHost, nauj_strng1);}
             if (counter == 2) {strcpy(myPort, nauj_strng1);}
             if (counter == 3) {strcpy(myLogin, nauj_strng1);}
             if (counter == 4) {strcpy(myPassword, nauj_strng1);}
             if (counter == 5) {strcpy(myCommand, nauj_strng1);}

             counter1 = counter;

             strcpy(nauj_strng, "");

          }

          strcpy(nauj_strng1, nauj_strng);
          sprintf(vns_simbl, "%c", *ptr);
          strcat(nauj_strng, vns_simbl);

          ptr++;

    }

    if (counter == 4) {strcpy(myCommand, nauj_strng);}
    return 0;
}

void showResult(void) {

    VMCHAR cmyHost[100] = {0};
    VMWCHAR cmyHostX[100];
    VMCHAR cmyPort[100] = {0};
    VMWCHAR cmyPortX[100];
    VMCHAR cmyLogin[100] = {0};
    VMWCHAR cmyLoginX[100];
    VMCHAR cmyPassword[100] = {0};
    VMWCHAR cmyPasswordX[100];
    VMCHAR cmyCommand[100] = {0};
    VMWCHAR cmyCommandX[100];

    if (trigeris != 1) {

       sprintf(cmyHost, "myHost: %s", myHost);
       vm_ascii_to_ucs2(cmyHostX, (strlen(cmyHost) + 1) * 2, cmyHost);
       vertical_scrolling_ucs2_text(cmyHostX);

       sprintf(cmyPort, "myPort: %s", myPort);
       vm_ascii_to_ucs2(cmyPortX, (strlen(cmyPort) + 1) * 2, cmyPort);
       vertical_scrolling_ucs2_text(cmyPortX);

       sprintf(cmyLogin, "myLogin: %s", myLogin);
       vm_ascii_to_ucs2(cmyLoginX, (strlen(cmyLogin) + 1) * 2, cmyLogin);
       vertical_scrolling_ucs2_text(cmyLoginX);

       sprintf(cmyPassword, "myPassword: %s", myPassword);
       vm_ascii_to_ucs2(cmyPasswordX, (strlen(cmyPassword) + 1) * 2, cmyPassword);
       vertical_scrolling_ucs2_text(cmyPasswordX);

       sprintf(cmyCommand, "myCommand: %s", myCommand);
       vm_ascii_to_ucs2(cmyCommandX, (strlen(cmyCommand) + 1) * 2, cmyCommand);
       vertical_scrolling_ucs2_text(cmyCommandX);

    } else {

       sprintf(cmyHost, "%s", "Not found config file:");
       vm_ascii_to_ucs2(cmyHostX, (strlen(cmyHost) + 1) * 2, cmyHost);
       vertical_scrolling_ucs2_text(cmyHostX);
       vertical_scrolling_ucs2_text(file_pathw);
    }

    filledDsplByLines = 0;  
 
}
