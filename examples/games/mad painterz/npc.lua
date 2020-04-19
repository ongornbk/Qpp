Npc = {}
Npcs = {}

Npcs.number = 0

function Npcs.Initialize(number)
Npcs.number = number
for i=0,Npcs.number,1
do
    Npcs[i] = Npc.new()
    Npcs[i].position.x = (Sunflower.width / 2)
    Npcs[i].position.y = (Sunflower.height / 2)
    Npcs[i].velocity.x = 0.0
    Npcs[i].velocity.y = 0.0
end
end

function Npcs.Render()
for i=0,Npcs.number,1
do
Npc.Render(Npcs[i])
end
end

function Npcs.Update(dt)
for i=0,Npcs.number,1
do
 Npc.Update(Npcs[i],dt)
end
end

function Npcs.ChangeDirection()
for i=0,Npcs.number,1
do
Npc.ChangeDirection(Npcs[i])
end
end

function Npcs.IntersectTest(player)

    for i=0,Npcs.number,1
    do

    Npc.Intersect(Npcs[i],player)

    end

end
    

function Npc.new()
return
{
    position = {},
    velocity = {},
    color = 7,
    ai = true,
    size = 32,
    sized2 = 16
}
end



function Npc.Update(npc,dt)

    npc.position.x = npc.position.x + npc.velocity.x
    npc.position.y = npc.position.y + npc.velocity.y

    if npc.position.x < 0
    then
        npc.position.x = Sunflower.width
    end

    if npc.position.x > Sunflower.width
    then
        npc.position.x = 0
    end

    if npc.position.y < 0
    then
        npc.position.y = Sunflower.height
    end

    if npc.position.y > Sunflower.height
    then
        npc.position.y = 0
    end

end

function Npc.ChangeDirection(npc)
    npc.velocity.x = math.random(-2.0,2.0)
    npc.velocity.y = math.random(-2.0,2.0)
    end

function Npc.Render(npc)

Sunflower.DrawRect(npc.position.x-npc.sized2,npc.position.y-npc.sized2,npc.position.x + npc.sized2,npc.position.y + npc.sized2,npc.color)
end

function Npc.Intersect(npc,player)

local dx = math.abs(npc.position.x  - player.position.x)
local dy = math.abs(npc.position.y - player.position.y)

local sd = npc.sized2 + player.sized2

if dx < sd and dy < sd
then
npc.color = player.color
end

end

