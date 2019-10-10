require "Sunflower/vector"

Sunflower = {}

Sunflower.client = {}
Sunflower.client.position = {}
Sunflower.client.position.left = 0
Sunflower.client.position.top = 0
Sunflower.client.position.bottom = 0
Sunflower.client.position.right = 0
Sunflower.position = {}
Sunflower.position.left = 0
Sunflower.position.top = 0
Sunflower.position.bottom = 0
Sunflower.position.right = 0
Sunflower.width = 0
Sunflower.height = 0
Sunflower.window = 0
Sunflower.hwnd = 0
Sunflower.dc = 0
Sunflower.hdc = 0
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
Sunflower.client.position.bottom, Sunflower.client.position.left, Sunflower.client.position.right, Sunflower.client.position.top = windows.GetClientRect(Sunflower.hwnd)
Sunflower.position.bottom, Sunflower.position.left, Sunflower.position.right, Sunflower.position.top = windows.GetRect(Sunflower.hwnd)
Sunflower.width = Sunflower.client.position.right - Sunflower.client.position.left
Sunflower.height = Sunflower.client.position.bottom - Sunflower.client.position.top
Sunflower.dc = windows.GetDC(Sunflower.hwnd)
Sunflower.paintstruct = windows.AllocPaintStruct()
Sunflower.hdc = windows.BeginPaint(Sunflower.hwnd,Sunflower.paintstruct)
Sunflower.buffer = windows.CreateCompatibleDC(Sunflower.dc)
end

function Sunflower.EndPaint()
windows.BitBlt(Sunflower.dc,Sunflower.client.position.left,Sunflower.client.position.top,Sunflower.client.position.right,Sunflower.client.position.bottom,Sunflower.buffer,0,0,Sunflower.raster.srccopy)
windows.DeleteDC(dc)
windows.DeleteDC(hdc)
Sunflower.dc = 0
Sunflower.hdc = 0
Sunflower.buffer = 0
windows.EndPaint(Sunflower.hwnd,Sunflower.paintstruct)
Sunflower.paintstruct = windows.FreePaintStruct(Sunflower.paintstruct)
end

function Sunflower.DrawPixel(x,y,color)
windows.SetPixel(Sunflower.hdc,x,y,color)
end

function Sunflower.GetTaskBarHeight()

if tonumber(get_setting("System.major")) >= 10
then

local taskBar = windows.Find("Shell_traywnd",0)
local bottom, left, right, top = windows.GetRect(taskBar)

if taskBar
then
return bottom - top
end
return -1
end
return 0
end

function Sunflower.ShowTaskBar(flag)
if tonumber(get_setting("System.major")) >= 10
then
local taskBar = windows.Find("Shell_traywnd", 0);
windows.Show(taskBar, flag);
end
end

function Sunflower.Fill(color)
windows.FillRect(Sunflower.dc,color,Sunflower.client.position.bottom,Sunflower.client.position.left,Sunflower.client.position.right,Sunflower.client.position.top)
end

function Sunflower.DrawRect(x1,y1,x2,y2,color)
    windows.FillRect(Sunflower.dc,color,y2,x1,x2,y1)
    end