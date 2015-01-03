//
//  main.c
//  TestAllegro
//
// Test harness to check building an Allegro native menu and setting the check box flags
// Malcolm Harrow, Dec 2014
//

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>

void handleError(char *e)
{
        printf("%s\n", e);
        exit(1);
}

int main(int argc, char **argv)
{
    if (!al_init()) handleError("Error al_init");
    if (!al_init_native_dialog_addon()) handleError("Error al_init_native_dialog_addon");
    
    
    ALLEGRO_DISPLAY *display = al_create_display(640, 480);
    if (!display) handleError("Error al_create_display");
    
    if (!al_install_keyboard()) handleError("Error al_install_keyboard");
    
    //    Building menu manually now works
    //
    //    ALLEGRO_MENU *menu = al_create_menu();
    //    ALLEGRO_MENU *file_menu = al_create_menu();
    //    ALLEGRO_MENU *recent_menu = al_create_menu();
    //
    //    if ((menu == NULL) || (file_menu == NULL) || (recent_menu == NULL)) handleError("Error al_create_menu");
    //
    //    al_append_menu_item(recent_menu, "Recent 1", 14, 0, NULL, NULL);
    //    al_append_menu_item(recent_menu, "Recent 2", 15, 0, NULL, NULL);
    //
    //    al_append_menu_item(file_menu, "&Open", 1, 0, NULL, NULL);
    //    al_append_menu_item(file_menu, "Open Recent...", 2, 0, NULL, recent_menu);
    //    al_append_menu_item(file_menu, "Exit", 3, 0, NULL, NULL);
    //
    //    al_append_menu_item(menu, "File", 0, 0, NULL, file_menu);
    
    ALLEGRO_MENU_INFO menu_info[] = {
        ALLEGRO_START_OF_MENU("&File", 1),
            { "&Open", 2, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
            ALLEGRO_START_OF_MENU("Open &Recent...", 3),
                { "Recent 1", 4, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
                { "Recent 2", 5, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
            ALLEGRO_END_OF_MENU,
            ALLEGRO_MENU_SEPARATOR,
            { "E&xit", 6, 0, NULL },
            ALLEGRO_END_OF_MENU,
            ALLEGRO_START_OF_MENU("&Help", 7),
                {"&About", 8, 0, NULL },
            ALLEGRO_END_OF_MENU,
        ALLEGRO_END_OF_MENU
    };

    ALLEGRO_MENU *menu = al_build_menu(menu_info);
    
    al_set_display_menu(display, menu);
    
    
    ALLEGRO_EVENT_QUEUE *events = al_create_event_queue();
    al_register_event_source(events, al_get_display_event_source(display));
    al_register_event_source(events, al_get_default_menu_event_source());
    
    printf("Flag ALLEGRO_MENU_ITEM_CHECKBOX %d\n", ALLEGRO_MENU_ITEM_CHECKBOX);
    printf("Flag ALLEGRO_MENU_ITEM_CHECKED %d\n", ALLEGRO_MENU_ITEM_CHECKED);
    printf("Flags | %d\n", ALLEGRO_MENU_ITEM_CHECKBOX | ALLEGRO_MENU_ITEM_CHECKED);
    printf("Try this:\n");
    printf("flag =1 checkbox but unchecked\n");
    int f;
    f = 1;
    f &= ~ALLEGRO_MENU_ITEM_CHECKBOX;
    printf("flags (1) &= ~ALLEGRO_MENU_ITEM_CHECKBOX, expect 0: %d\n", f);
    f = 2;
    f &= ~ALLEGRO_MENU_ITEM_CHECKBOX;
    printf("flags (2) &= ~ALLEGRO_MENU_ITEM_CHECKBOX, expect 2: %d\n", f);
    f = 3;
    f &= ~ALLEGRO_MENU_ITEM_CHECKBOX;
    printf("flags (3) &= ~ALLEGRO_MENU_ITEM_CHECKBOX, expect 2: %d\n", f);
    
    printf("Should have a flag of 1 as defined as a checkbox. 1=%d\n", al_get_menu_item_flags(menu, 2));
    f = al_toggle_menu_item_flags(menu, 2, ALLEGRO_MENU_ITEM_CHECKED);
    printf("After toggle of CHECKED flag, should be 3=%d\n", al_get_menu_item_flags(menu, 2));
    printf("value of f is %d\n", f);
    al_set_menu_item_flags(menu, 2, ALLEGRO_MENU_ITEM_CHECKED | ALLEGRO_MENU_ITEM_CHECKBOX);
    printf("After using set with a bitwise or should be 3= %d\n", al_get_menu_item_flags(menu, 2));
    al_set_menu_item_flags(menu, 2, 3);
    printf("After directly setting the flags to 3, value should be 3=%d\n", al_get_menu_item_flags(menu, 2));
    al_set_menu_item_flags(menu, 2, ALLEGRO_MENU_ITEM_CHECKBOX);
    printf("After using set to not state CHECKED, value should be 1=%d\n", al_get_menu_item_flags(menu, 2));
    f = al_toggle_menu_item_flags(menu, 6, ALLEGRO_MENU_ITEM_CHECKED);
    printf("value of f is %d\n", f);
    f = al_toggle_menu_item_flags(menu, 4, ALLEGRO_MENU_ITEM_CHECKED);
    printf("Set recent 1 value of f is %d\n", f);
    bool quited = false;
    
    while (!quited)
    {
        ALLEGRO_EVENT event;
        
        al_wait_for_event(events, &event);
        switch (event.type)
        {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                quited = true;
                break;
            case ALLEGRO_EVENT_MENU_CLICK:
                printf("Clicked %ld\n", event.user.data1);
                if (event.user.data1 == 6) quited = true;
                if (event.user.data1 == 4)
                {
                   f = al_toggle_menu_item_flags(menu, 5, ALLEGRO_MENU_ITEM_CHECKED);
                    printf("Set recent 2 value of f is %d\n", f);

                }
                if (event.user.data1 == 5)
                {
                    f = al_toggle_menu_item_flags(menu, 4, ALLEGRO_MENU_ITEM_CHECKED);
                    printf("Set recent 1 value of f is %d\n", f);
                }
                break;
        }
    }
   
    return 0;
}