require "Sunflower/vector"

Sunflower = {}

Sunflower.window = 0
Sunflower.hwnd = 0
Sunflower.dc = 0
Sunflower.lpmsg = 0
Sunflower.msg = 0
Sunflower.wparam = 0
Sunflower.lparam = 0
Sunflower.result = 0
Sunflower.timers = vector.new()
Sunflower.paintstruct = 0
Sunflower.buffer = 0
Sunflower.raster = {}

Sunflower.raster.srccopy = 0x00CC0020

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
Sunflower.result = windows.Proc(Sunflower.hwnd,Sunflower.msg,Sunflower.wparam,Sunflower.lparam)
end

function Sunflower.Close()
windows.DestroyWindow(Sunflower.window)

for i = 0,Sunflower.timers.size-1,1
do
windows.KillTimer(Sunflower.hwnd,vector.get(Sunflower.timers,i))
end

vector.delete(Sunflower.timers)
end

function Sunflower.HideMenu()
windows.HideMenu(Sunflower.hwnd)
end

function Sunflower.OnClose(foo)
windows.RegisterEvent(0x0010,foo)
end

function Sunflower.OnPaint(foo)
windows.RegisterEvent(0x000f,foo)
end

function Sunflower.OnHover(foo)
windows.RegisterEvent(0x02a1,foo)
end

function Sunflower.BeginPaint()
Sunflower.dc = windows.GetDC(Sunflower.hwnd)
Sunflower.paintstruct = windows.AllocPaintStruct()
windows.BeginPaint(Sunflower.hwnd,Sunflower.paintstruct)
Sunflower.buffer = windows.CreateCompatibleDC(Sunflower.dc)
end

function Sunflower.EndPaint()
windows.BitBlt(Sunflower.dc,0,0,50,50,Sunflower.buffer,0,0,Sunflower.raster.srccopy)
windows.DeleteDC(dc)
windows.DeleteDC(buffer)
Sunflower.dc = 0
Sunflower.buffer = 0
windows.EndPaint(Sunflower.hwnd,Sunflower.paintstruct)
Sunflower.paintstruct = windows.FreePaintStruct(Sunflower.paintstruct)
end

function Sunflower.DrawPixel(x,y,color)
windows.SetPixel(Sunflower.buffer,x,y,color)
end