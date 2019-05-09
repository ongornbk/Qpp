local style = tonumber(get_setting("credits.color"))

function Credits()

local guard = true

system.System("cls")

    while guard == true
    do
        conio.SetColor(11)
        local string_style = " style: " .. style .. " "
        conio.Gotoxy(40- string.len(string_style,0))
        conio.Print(string_style)
        conio.Gotoxy(0,1)
        conio.SetColor(style)
     conio.FWrite(" __       __                  __                  _______                       \n",81)
     conio.FWrite("|  \\     /  \\                |  \\                |       \\                      \n",81)
     conio.FWrite("| $$\\   /  $$  ______    ____| $$  ______        | $$$$$$$\\ __    __            \n",81)
     conio.FWrite("| $$$\\ /  $$$ |      \\  /      $$ /      \\       | $$__/ $$|  \\  |  \\           \n",81)
     conio.FWrite("| $$$$\\  $$$$  \\$$$$$$\\|  $$$$$$$|  $$$$$$\\      | $$    $$| $$  | $$           \n",81)
     conio.FWrite("| $$\\$$ $$ $$ /      $$| $$  | $$| $$    $$      | $$$$$$$\\| $$  | $$           \n",81)                
     conio.FWrite("| $$ \\$$$| $$|  $$$$$$$| $$__| $$| $$$$$$$$      | $$__/ $$| $$__/ $$           \n",81)
     conio.FWrite("| $$  \\$ | $$ \\$$    $$ \\$$    $$ \\$$     \\      | $$    $$ \\$$    $$           \n",81)
     conio.FWrite(" \\$$      \\$$  \\$$$$$$$  \\$$$$$$$  \\$$$$$$$       \\$$$$$$$  _\\$$$$$$$           \n",81)
     conio.FWrite("                                                           |  \\__| $$           \n",81)
     conio.FWrite("                                                            \\$$    $$           \n",81)
     conio.FWrite("                                                             \\$$$$$$            \n",81)
     conio.FWrite("  ______                                                     __        __       \n",81)
     conio.FWrite(" /      \\                                                   |  \\      |  \\      \n",81)
     conio.FWrite("|  $$$$$$\\ _______    ______    ______    ______   _______  | $$____  | $$   __ \n",81)
     conio.FWrite("| $$  | $$|       \\  /      \\  /      \\  /      \\ |       \\ | $$    \\ | $$  /  \\\n",81)
     conio.FWrite("| $$  | $$| $$$$$$$\\|  $$$$$$\\|  $$$$$$\\|  $$$$$$\\| $$$$$$$\\| $$$$$$$\\| $$_/  $$\n",81)
     conio.FWrite("| $$  | $$| $$  | $$| $$  | $$| $$  | $$| $$    $$| $$  | $$| $$  | $$| $$   $$ \n",81)
     conio.FWrite("| $$__/ $$| $$  | $$| $$__| $$| $$__/ $$| $$$$$$$$| $$  | $$| $$__/ $$| $$$$$$\\ \n",81)
     conio.FWrite(" \\$$    $$| $$  | $$ \\$$    $$ \\$$    $$ \\$$     \\| $$  | $$| $$    $$| $$  \\$$\\\n",81)
     conio.FWrite("  \\$$$$$$  \\$$   \\$$ _\\$$$$$$$  \\$$$$$$   \\$$$$$$$ \\$$   \\$$ \\$$$$$$$  \\$$   \\$$\n",81)
     conio.FWrite("                    |  \\__| $$                                                  \n",81)
     conio.FWrite("                     \\$$    $$                                                  \n",81)
     conio.FWrite("                      \\$$$$$$                                                   \n",81)

     if(win.KeyDown(0x25))
     then
         while (win.KeyDown(0x25))
     do
         sleep(50000)
     end
         style = style - 1

         if style < 0
         then
            style = 0
         end
     end

     if(win.KeyDown(0x27))
     then
         while (win.KeyDown(0x27))
     do
         sleep(50000)
     end
         style = style + 1

         if style > 255
         then
            style = 255
         end
     end

    if(win.KeyDown(0x1B) or win.KeyDown(0x0D))
    then
        while (win.KeyDown(0x0D))
    do
        sleep(50000)
    end
        conio.SetColor(15)
        system.System("cls")
        guard = false
    end
    end
    
end