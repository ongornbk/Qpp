
MenuItem = {}

function MenuItem.new()
return
{
name = "",
items = {},
ref = 0,
size = 0,
ptr = " <--    ",
cls = "    ",
selectable = false,
selection = 0
}
end

function MenuItem.release(this)

end

function MenuItem.Draw(this)
conio.Println(this.name)
end

function MenuItem.DrawPtr(this)
conio.Print(this.ptr .. "    ")
end

function MenuItem.Clean(this)
    if this.selectable == false
    then
conio.Print(this.cls)
    end
end

function MenuItem.Select(this)
    
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