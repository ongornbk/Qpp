function floorPosition(x,y)
    conio.Gotoxy(math.floor(x),math.floor(y))
    end

    function saveSettings()
    
        filesystem.Remove("settings.ini")
        
        filesystem.OpenTrunc("settings.ini")
    
        filesystem.AppendString("width " .. tostring(get_setting("width")).. "\n")
    
        filesystem.AppendString("height " .. tostring(get_setting("height")).. "\n")
        
        filesystem.AppendString("difficulty " .. tostring(get_setting("difficulty")) .. "\n")
    
        filesystem.AppendString("sounds " .. tostring(get_setting("sounds")) .. "\n")
    
        filesystem.AppendString("initialized " .. tostring(get_setting("initialized")))
    
        filesystem.Close()
        
        end
    
        if initialized == 0
    then
        Initialize(initialized)
        initialized = 1
        saveSettings()
    end