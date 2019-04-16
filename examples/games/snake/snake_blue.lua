SnakeBlue = {}

SnakeBlue.element = {}

SnakeBlue.element[0] = {}

SnakeBlue.element[0].x = 5
SnakeBlue.element[0].y = 5

SnakeBlue.size = 0

SnakeBlue.velocity = {}

SnakeBlue.velocity.x = 0
SnakeBlue.velocity.y = 0

SnakeBlue.lastposition = {}

SnakeBlue.lastposition.x = 5
SnakeBlue.lastposition.y = 5

function SnakeBlue.Enlarge ()
    SnakeBlue.size = SnakeBlue.size + 1
    SnakeBlue.element[SnakeBlue.size] = {}
    SnakeBlue.element[SnakeBlue.size].x = SnakeBlue.element[SnakeBlue.size-1].x
    SnakeBlue.element[SnakeBlue.size].y = SnakeBlue.element[SnakeBlue.size-1].y
  end
  
function SnakeBlue.Collision(x,y)

    local collision = 0
for i = 1,SnakeBlue.size,1
do

    if(x == SnakeBlue.element[i].x and y == SnakeBlue.element[i].y)
    then
        collision = i
        break;
    end
end

if(collision > 0)
then
for i = collision,SnakeBlue.size,1
do
   map.Print(SnakeBlue.element[i].x,SnakeBlue.element[i].y," ")
end

SnakeBlue.size = collision

end

end

function SnakeBlue.Draw()

    con.SetColor(11)
    map.Print(SnakeBlue.element[0].x,SnakeBlue.element[0].y,"@")

for i = 1,SnakeBlue.size-1,1
do
    local char = tostring(i%10)

    if(SnakeBlue.element[i].x == map.food.x and SnakeBlue.element[i].y == map.food.y)
    then
      char = "X"
    end
    map.Print(SnakeBlue.element[i].x,SnakeBlue.element[i].y,char)
end

    if(SnakeBlue.size > 0)
    then
    map.Print(SnakeBlue.element[SnakeBlue.size].x,SnakeBlue.element[SnakeBlue.size].y,"*")
    end

    map.Print(SnakeBlue.lastposition.x,SnakeBlue.lastposition.y," ")
end

function SnakeBlue.Update()

    if(win.KeyDown(0x26))
then
    SnakeBlue.velocity.x = 0
    SnakeBlue.velocity.y = -1
else

    if(win.KeyDown(0x27))
    then
        SnakeBlue.velocity.x = 1
        SnakeBlue.velocity.y = 0
    else

        if(win.KeyDown(0x25))
        then
            SnakeBlue.velocity.x = -1
            SnakeBlue.velocity.y = 0
        else

            if(win.KeyDown(0x28))
            then
                SnakeBlue.velocity.x = 0
                SnakeBlue.velocity.y = 1
            end

        end

    end

end

    SnakeBlue.lastposition.x = SnakeBlue.element[SnakeBlue.size].x
    SnakeBlue.lastposition.y = SnakeBlue.element[SnakeBlue.size].y

for i = 0,SnakeBlue.size-1,1
do
    SnakeBlue.element[SnakeBlue.size-i].x = SnakeBlue.element[SnakeBlue.size - i - 1].x
    SnakeBlue.element[SnakeBlue.size-i].y = SnakeBlue.element[SnakeBlue.size - i - 1].y
end

SnakeBlue.element[0].x = SnakeBlue.element[0].x + SnakeBlue.velocity.x
SnakeBlue.element[0].y = SnakeBlue.element[0].y + SnakeBlue.velocity.y

if(SnakeBlue.element[0].x > map.width)
then
    SnakeBlue.element[0].x = 1
else

if(SnakeBlue.element[0].x < 1)
then
    SnakeBlue.element[0].x = map.width
end

end

if(SnakeBlue.element[0].y > map.height)
then
    SnakeBlue.element[0].y = 1
else

if(SnakeBlue.element[0].y < 1)
then
    SnakeBlue.element[0].y = map.height
end

if(SnakeBlue.element[0].x == map.food.x and SnakeBlue.element[0].y == map.food.y)
then

con.SetColor(7)
map.Print(0,map.food.y,"|")
map.Print(map.food.x,0,"-")
map.Print(map.width+1,map.food.y,"|")
map.Print(map.food.x,map.height+1,"-")

SnakeBlue.Enlarge()
map.CreateFood()

map.Print(0,map.food.y,"+")
map.Print(map.food.x,0,"+")
map.Print(map.width+1,map.food.y,"+")
map.Print(map.food.x,map.height+1,"+")
end

SnakeBlue.Collision(SnakeBlue.element[0].x,SnakeBlue.element[0].y)
SnakeRed.Collision(SnakeBlue.element[0].x,SnakeBlue.element[0].y)
end

end