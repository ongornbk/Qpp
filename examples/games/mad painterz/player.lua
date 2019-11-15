Player = {}

function Player.new()
return
{
    position = {},
    velocity = {},
    color = 0,
    key = {},
    ai = false,
    size = 0,
    sized2 = 0
}
end

function Player.Update(player,dt)

    if(windows.KeyDown(player.key.left))
    then
        player.velocity.x = -5

    else

    if(windows.KeyDown(player.key.right))
    then
        player.velocity.x = 5
    else
        player.velocity.x = 0
    end
end

if(windows.KeyDown(player.key.top))
    then
        player.velocity.y = -5

    else

    if(windows.KeyDown(player.key.bottom))
    then
        player.velocity.y = 5
    else
        player.velocity.y = 0
    end
end

    player.position.x = player.position.x + player.velocity.x
    player.position.y = player.position.y + player.velocity.y

    if player.position.x < 0
    then
        player.position.x = Sunflower.width
    end

    if player.position.x > Sunflower.width
    then
        player.position.x = 0
    end

    if player.position.y < 0
    then
        player.position.y = Sunflower.height
    end

    if player.position.y > Sunflower.height
    then
        player.position.y = 0
    end

end

function Player.Render(player)
Sunflower.DrawRect(player.position.x-player.sized2,player.position.y-player.sized2,player.position.x + player.sized2,player.position.y + player.sized2,player.color)
end