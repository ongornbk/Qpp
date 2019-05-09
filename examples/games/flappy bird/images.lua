
function loadimage(image,width,height)

    for x = 0,width-1,1
    do
    
        for y = 0,height-1,1
        do
        Image.set(image,x,y,177)
        end
    
    end
    
    for x = 0,width-1,1
    do
    
        for y = 30+(x%3)+math.random(0,1),height-1,1
        do
        Image.set(image,x,y,37)
        end
    
    end
    
    for y = 0,width-1,1
    do
        Image.set(image,82,y,99)
        Image.set(image,width,y,99)
        Image.set(image,width-1,y,99)
    end
    
    Image.set(image,65,10,224)
    Image.set(image,66,10,224)
    Image.set(image,67,10,224)
    Image.set(image,68,10,224)
    Image.set(image,69,10,224)
    Image.set(image,70,10,224)
    Image.set(image,66,11,224)
    Image.set(image,67,11,224)
    Image.set(image,68,11,224)
    Image.set(image,69,11,224)
    Image.set(image,66,9,224)
    Image.set(image,67,9,224)
    Image.set(image,68,9,224)
    Image.set(image,69,9,224)
    
    Image.set(image,35,10,112)
    Image.set(image,36,10,112)
    Image.set(image,37,10,112)
    Image.set(image,38,10,112)
    Image.set(image,39,10,112)
    Image.set(image,34,10,112)
    Image.set(image,36,11,112)
    Image.set(image,37,11,112)
    Image.set(image,38,11,112)
    Image.set(image,39,11,112)
    Image.set(image,36,9,112)
    Image.set(image,37,9,112)
    Image.set(image,38,9,112)
    Image.set(image,39,9,112)
    Image.set(image,36,8,112)
    Image.set(image,37,8,112)
    Image.set(image,38,8,112)
    Image.set(image,39,7,112)

end