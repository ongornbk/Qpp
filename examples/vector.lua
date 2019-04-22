
vector = {}

function vector.new()
return
{
    size = 0,
    capacity = 2,
    element_size = 8,
    data = malloc(16)
}
end

function vector.delete(vector)
free(data)
end

function vector.get(vector,element)

    if(element < vector.size)
    then
        return get(vector.data,element)
    else
        return -1
    end
end

function vector.set(vector,element,value)
    if(element < vector.size)
    then
       set(vector.data,element,value)
    end
end

function vector.push_back(vector,value)

    vector.size = vector.size + 1

    if(vector.size >= vector.capacity)
    then
        vector.capacity = vector.capacity * 2
        vector.data = realloc(vector.data,vector.capacity * 8)
    end

    set(vector.data,vector.size - 1,value)

end