SnakeRed = {}

SnakeRed.element = {}

SnakeRed.element[0] = {}

SnakeRed.element[0].x = 25
SnakeRed.element[0].y = 15

SnakeRed.size = 0

SnakeRed.velocity = {}

SnakeRed.velocity.x = 0
SnakeRed.velocity.y = 0

SnakeRed.lastposition = {}

SnakeRed.lastposition.x = 5
SnakeRed.lastposition.y = 5

function SnakeRed.Enlarge ()
    SnakeRed.size = SnakeRed.size + 1
    SnakeRed.element[SnakeRed.size] = {}
    SnakeRed.element[SnakeRed.size].x = SnakeRed.element[SnakeRed.size-1].x
    SnakeRed.element[SnakeRed.size].y = SnakeRed.element[SnakeRed.size-1].y
  end
  
function SnakeRed.Collision(x,y)

    local collision = 0
for i = 1,SnakeRed.size,1
do

    if(x == SnakeRed.element[i].x and y == SnakeRed.element[i].y)
    then
        collision = i
        break;
    end
end

if(collision > 0)
then
for i = collision,SnakeRed.size,1
do
   map.Print(SnakeRed.element[i].x,SnakeRed.element[i].y," ")
end

SnakeRed.size = collision

end

end

function SnakeRed.Draw()

    con.SetColor(12)
    map.Print(SnakeRed.element[0].x,SnakeRed.element[0].y,"@")

for i = 1,SnakeRed.size-1,1
do
    local char = tostring(i%10)

    if(SnakeRed.element[i].x == map.food.x and SnakeRed.element[i].y == map.food.y)
    then
      char = "X"
    end
    map.Print(SnakeRed.element[i].x,SnakeRed.element[i].y,char)
end

    if(SnakeRed.size > 0)
    then
    map.Print(SnakeRed.element[SnakeRed.size].x,SnakeRed.element[SnakeRed.size].y,"*")
    end

    map.Print(SnakeRed.lastposition.x,SnakeRed.lastposition.y," ")
end

function SnakeRed.Update()

    if(win.KeyDown(0x57))
then
    SnakeRed.velocity.x = 0
    SnakeRed.velocity.y = -1
else

    if(win.KeyDown(0x44))
    then
        SnakeRed.velocity.x = 1
        SnakeRed.velocity.y = 0
    else

        if(win.KeyDown(0x41))
        then
            SnakeRed.velocity.x = -1
            SnakeRed.velocity.y = 0
        else

            if(win.KeyDown(0x53))
            then
                SnakeRed.velocity.x = 0
                SnakeRed.velocity.y = 1
            end

        end

    end

end

    SnakeRed.lastposition.x = SnakeRed.element[SnakeRed.size].x
    SnakeRed.lastposition.y = SnakeRed.element[SnakeRed.size].y

for i = 0,SnakeRed.size-1,1
do
    SnakeRed.element[SnakeRed.size-i].x = SnakeRed.element[SnakeRed.size - i - 1].x
    SnakeRed.element[SnakeRed.size-i].y = SnakeRed.element[SnakeRed.size - i - 1].y
end

SnakeRed.element[0].x = SnakeRed.element[0].x + SnakeRed.velocity.x
SnakeRed.element[0].y = SnakeRed.element[0].y + SnakeRed.velocity.y

if(SnakeRed.element[0].x > map.width)
then
    SnakeRed.element[0].x = 1
else

if(SnakeRed.element[0].x < 1)
then
    SnakeRed.element[0].x = map.width
end

end

if(SnakeRed.element[0].y > map.height)
then
    SnakeRed.element[0].y = 1
else

if(SnakeRed.element[0].y < 1)
then
    SnakeRed.element[0].y = map.height
end

if(SnakeRed.element[0].x == map.food.x and SnakeRed.element[0].y == map.food.y)
then

con.SetColor(7)
map.Print(0,map.food.y,"|")
map.Print(map.food.x,0,"-")
map.Print(map.width+1,map.food.y,"|")
map.Print(map.food.x,map.height+1,"-")

SnakeRed.Enlarge()
map.CreateFood()

map.Print(0,map.food.y,"+")
map.Print(map.food.x,0,"+")
map.Print(map.width+1,map.food.y,"+")
map.Print(map.food.x,map.height+1,"+")
end

SnakeRed.Collision(SnakeRed.element[0].x,SnakeRed.element[0].y)
SnakeBlue.Collision(SnakeRed.element[0].x,SnakeRed.element[0].y)
end

end