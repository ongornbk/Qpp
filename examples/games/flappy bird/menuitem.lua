
MenuItem = {}

function MenuItem.new()
return
{
name = "",
items = {},
ref = 0,
size = 0,
ptr = " " .. tostring(get_setting("menu.pointer")),
ptrsize = string.len(tostring(get_setting("menu.pointer"))),
cls = " ",
selectable = false,
selection = 0,
selected = false,
colors = {}
}
end

function MenuItem.release(this)
this.items = nil
end

function MenuItem.Draw(this,selected)
if this.selectable == true
then
    conio.Println(this.name  .. this.ptr  .. "         ")
else
    conio.Println(this.name .. "          ")
end
end

function MenuItem.DrawPtr(this)
    if this.selectable == true and this.selected == true
    then
        conio.Print("[")
        conio.SetColor(this.colors[this.selection])
        conio.Print(this.ptr)
        conio.SetColor(tonumber(get_setting("font.color")))
        conio.Print("]       ") 
    else
conio.Print(this.ptr .. "        ")
    end
    this.selected = false
end

function MenuItem.Clean(this)
    if this.selectable == false
    then
        conio.FPutc(this.cls)
        for i = 1,this.ptrsize, 1
        do
            conio.FPutc(this.cls)
        end
    end
end

function MenuItem.Select(this)
    this.selected = true
    if this.selectable == true
    then
this.ptr = this.items[this.selection]
if this.ref ~= nil and this.ref ~= 0
then
set(this.ref,0,this.selection)
end
    end

end

function MenuItem.PushItem(this,item)
    this.items[this.size] = item
    this.colors[this.size] = 15
    this.size = this.size + 1
    
end

function MenuItem.PushColoredItem(this,item,color)
    this.items[this.size] = item
    this.colors[this.size] = color
    this.size = this.size + 1
end

function MenuItem.Decrement(this)
    if this.selection > 0
    then
    this.selection = this.selection - 1
    end
end

function MenuItem.Increment(this)
    if this.selection < this.size - 1
    then
    this.selection = this.selection + 1
    end
end