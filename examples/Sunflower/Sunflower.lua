Sunflower = {}

Sunflower.msg = 0
Sunflower.wparam = 0
Sunflower.lparam = 0

function Sunflower.Start(show)
CreateWindow()
ShowWindow(show)
end

function Sunflower.Run()
local val = GetMessage()
Sunflower.lparam,Sunflower.wparam,Sunflower.msg = RetrieveMessage()
return val
end

function Sunflower.Peek()
local val = PeekMessage()
Sunflower.lparam,Sunflower.wparam,Sunflower.msg = RetrieveMessage()
return val
end

function Sunflower.Proc()
WindowProc(Sunflower.msg,Sunflower.wparam,Sunflower.lparam)
end