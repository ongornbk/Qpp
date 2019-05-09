
if background ~= 0
then
Image.release(background)
end


background = Image.new(math.floor(width),math.floor(height))
for x = 0,width-1,1
do

    for y = 0,height-1,1
    do
    Image.set(background,x,y,177)
    end

end

for x = 0,width-1,1
do

    for y = 30+(x%3)+math.random(0,1),height-1,1
    do
    Image.set(background,x,y,37)
    end

end

for y = 0,width-1,1
do
    Image.set(background,82,y,99)
    Image.set(background,width,y,99)
    Image.set(background,width-1,y,99)
end

Image.set(background,65,10,224)
Image.set(background,66,10,224)
Image.set(background,67,10,224)
Image.set(background,68,10,224)
Image.set(background,69,10,224)
Image.set(background,70,10,224)
Image.set(background,66,11,224)
Image.set(background,67,11,224)
Image.set(background,68,11,224)
Image.set(background,69,11,224)
Image.set(background,66,9,224)
Image.set(background,67,9,224)
Image.set(background,68,9,224)
Image.set(background,69,9,224)

Image.set(background,35,10,112)
Image.set(background,36,10,112)
Image.set(background,37,10,112)
Image.set(background,38,10,112)
Image.set(background,39,10,112)
Image.set(background,34,10,112)
Image.set(background,36,11,112)
Image.set(background,37,11,112)
Image.set(background,38,11,112)
Image.set(background,39,11,112)
Image.set(background,36,9,112)
Image.set(background,37,9,112)
Image.set(background,38,9,112)
Image.set(background,39,9,112)
Image.set(background,36,8,112)
Image.set(background,37,8,112)
Image.set(background,38,8,112)
Image.set(background,39,7,112)