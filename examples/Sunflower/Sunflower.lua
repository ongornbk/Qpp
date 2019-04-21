Sunflower = {}

Sunflower.window = 0
Sunflower.hwnd = 0
Sunflower.msg = 0
Sunflower.wparam = 0
Sunflower.lparam = 0

function Sunflower.Start(show)
Sunflower.msg = malloc(48)
bzero(Sunflower.msg)
Sunflower.window = windows.CreateWindow()
Sunflower.hwnd = windows.GetWindowHandle(Sunflower.window)
windows.Show(Sunflower.hwnd,show)
end

function Sunflower.Run()
--local val = GetMessage()
--Sunflower.lparam,Sunflower.wparam,Sunflower.msg = RetrieveMessage()
--return val
end

function Sunflower.Peek()
--local val = PeekMessage()
--Sunflower.lparam,Sunflower.wparam,Sunflower.msg = RetrieveMessage()
--return val
end

function Sunflower.Proc()
windows.Proc(Sunflower.hwnd,Sunflower.msg,Sunflower.wparam,Sunflower.lparam)
end

function Sunflower.Close()
windows.DestroyWindow(Sunflower.hwnd)
free(Sunflower.msg)
release(Sunflower.msg)
release(Sunflower.window)
release(Sunflower.hwnd)
end