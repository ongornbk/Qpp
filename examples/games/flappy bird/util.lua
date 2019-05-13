function floorPosition(x,y)
    conio.Gotoxy(math.floor(x),math.floor(y))
    end

    function saveSettings()
    
        filesystem.Remove("settings.ini")
        
        filesystem.OpenTrunc("settings.ini")
    
        filesystem.AppendString("width " .. tostring(width).. "\n")
    
        filesystem.AppendString("height " .. tostring(height).. "\n")
        
        filesystem.AppendString("difficulty " .. tostring(difficulty) .. "\n")
    
        filesystem.AppendString("sounds " .. tostring(sounds) .. "\n")
    
        filesystem.AppendString("initialized " .. tostring(initialized))
    
        filesystem.Close()
        
        end
    
        if initialized == 0
    then
        Initialize(initialized)
        initialized = 1
        saveSettings()
    end