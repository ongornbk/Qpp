
require "column"

Map = {}

function Map.new()
   return
   {
       matrix = {},
       sizex = 0,
       sizey = 0,
       color = 0,
       default_color = 0,
       border_color = 0,
       velocity = {},
       width = 0,
       distance = 0,
       iteration = 0,
       margin_top = 0,
       gap = 0,
       offset = 0,
       lasty = 0,
       min = 0,
       max = 0
   } 
end

function Map.Initialize(this)

    for i = 0,this.sizex,1
    do
        this.matrix[i] = column.new(i,this.width)
    end

    this.width_iteration = this.width
    this.offset = (this.gap+(this.gap%2)) / 2

    conio.SetColor(this.border_color)

    for x = 0, this.sizex,1
    do
        conio.Gotoxy(x,0)
        conio.Print(" ")
        conio.Gotoxy(x,this.sizey+1)
        conio.Print(" ")
    end

    for y = 0, this.sizey+1,1
    do
        conio.Gotoxy(0,y)
        conio.Print("  ")
        conio.Gotoxy(this.sizex+1,y)
        conio.Print("  ")
    end

    for x = 2, this.sizex,1
    do
        for y = 1, this.sizey-1,1
    do
        conio.Gotoxy(x,y)
        conio.SetColor(Image.get(background,x-1,y))
        conio.Print(" ")

    end
    end

    for x = 2, this.sizex+this.width-4,1
    do
        conio.Gotoxy(x,this.sizey)
        conio.SetColor(Image.get(background,x,this.sizey-1))
        conio.Print(" ")
    end
    
end

function Map.Update(this)

this.iteration = this.iteration + 1

for i = 0,this.sizex - 1,1
do
this.matrix[i].rendering = this.matrix[i+1].rendering
this.matrix[i].y = this.matrix[i+1].y

end

if this.iteration % this.distance == 0
then
    this.matrix[this.sizex-1].rendering = true
    this.matrix[this.sizex-1].y = math.random(this.min,this.max)
    this.width_iteration = this.width_iteration - 1
end

end

function Map.Draw(this)

for x = 0,this.sizex,1
do
if this.matrix[x].rendering == true
then

column.Draw(this.matrix[x],this.color,this.default_color,this.offset,this.sizey,this.margin_top)


end

end 

end

function Map.NormalizePosition(this,actor)

    if(actor.position.y > this.sizey -1)
    then
        actor.position.y = this.sizey -1
    else if(actor.position.y < this.margin_top)
        then
            actor.position.y = this.margin_top
        end

    end

end

function Map.CheckCollision(this,actor)

local index = math.floor(actor.position.x+4)

if(this.matrix[index].rendering == true)
then

if (actor.position.y < (this.matrix[index].y - this.offset))
then
return true
end

if (actor.position.y > (this.matrix[index].y + this.offset))
then
return true
end

score = score + 1
if sounds == 1
then
winmm.PlayAsync("assets//score.wav")
end

end

return false    

end