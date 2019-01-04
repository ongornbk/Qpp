Sunflower = {}

Sunflower.msg = 0

function Sunflower.Start(show)
CreateWindow()
ShowWindow(show)
end

function Sunflower.SetIcon(icon) --notworking
local iptrA,iptrB = LoadIcon(icon)
SendMessage(0x0080,0,iptrA,iptrB)
end

function Sunflower.Run()
local val = GetMessage()
Sunflower.lparam,Sunflower.wparam,Sunflower.msg = RetrieveMessage()
return val
end

function Sunflower.Proc()
WindowProc(msg,wparam,lparam)
end