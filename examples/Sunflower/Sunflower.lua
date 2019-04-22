require "vector"

Sunflower = {}

Sunflower.window = 0
Sunflower.hwnd = 0
Sunflower.lpmsg = 0
Sunflower.msg = 0
Sunflower.wparam = 0
Sunflower.lparam = 0
Sunflower.timers = vector.new()

function Sunflower.Start(title,show)
Sunflower.lpmsg = malloc(48)
bzero(Sunflower.lpmsg)
Sunflower.window = windows.CreateWindow()
Sunflower.hwnd = windows.GetWindowHandle(Sunflower.window)
windows.SetTitle(Sunflower.hwnd,title)
windows.Show(Sunflower.hwnd,show)
end

function Sunflower.Init()
    Sunflower.lpmsg = malloc(48)
    bzero(Sunflower.lpmsg)
end

function Sunflower.Run()
local val = windows.GetMessage(Sunflower.hwnd,Sunflower.lpmsg)
Sunflower.lparam,Sunflower.wparam,Sunflower.msg = windows.RetrieveMessage(Sunflower.lpmsg)
return val
end

function Sunflower.Peek()
local val = windows.PeekMessage(Sunflower.hwnd,Sunflower.lpmsg)
Sunflower.lparam,Sunflower.wparam,Sunflower.msg = windows.RetrieveMessage(Sunflower.lpmsg)
return val
end

function Sunflower.SetTimer(id,time)
windows.SetTimer(Sunflower.hwnd,time,id)
vector.push_back(Sunflower.timers,id)
end

function Sunflower.Proc()
windows.Proc(Sunflower.hwnd,Sunflower.msg,Sunflower.wparam,Sunflower.lparam)
end

function Sunflower.Close()
windows.DestroyWindow(Sunflower.window)

for i = 0,vec.size-1,1
do
windows.KillTimer(Sunflower.hwnd,vector.get(vec,i))
end

vector.delete(Sunflower.timers)
end