
local num_results = 0

function iow(str)
fs.AppendString(str)
end

function iowl(str)
fs.AppendString(str .. "\n")
end



function UnpackItem(item)

num_results = num_results + 1

title1,title2 = string.find(item,"dn=")
title3,title4 = string.find(item,"&tr=")
local title = string.sub(item,title2+1,title3-1)
title = string.gsub(title,"+"," ")
iowl("<tr>")
iowl("<td>")
iowl(tostring(num_results))
iowl("</td>")
iowl("<td>")
iowl(tostring(title))
iowl("</td>")
iowl("<td>")
iow("<a href=")
iow("\"magnet:?xt=urn:btih:")
iowl(item)
iowl("\">")
iowl("Copy Link!")
iowl("</a>")
iowl("</td>")

iowl("</tr>")
end

function UnpackResult(result,itemname,source)

num_results = 0

fs.OpenAppend("magnets.html")

iowl("<html>")
iowl("<head>")
iowl("<meta charset=\"utf-8\">")
iowl("<link rel=\"stylesheet\" href=\"style.css\">")
iowl("<link rel=\"icon\" type=\"image/png\" href=\"favicon.ico\"/>")
iowl("<title>Szrot Downloader</title>")
iowl("<meta name=\"author\" content=\"ongornbk\" />")
iowl("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, minimum-scale=1.0, maximum-scale=1.0, user-scalable=no\">")
iowl("</head>")
iowl("<body>")
iowl("<div id=\"container\">")
iowl("<!-- particles.js container -->")
iowl("<div id=\"particles-js\"></div>")
iowl("<!-- scripts -->")
iowl("<script src=\"js/particles.js\"></script>")
iowl("<script src=\"js/app.js\"></script>")
iowl("<!-- stats.js -->")
iowl("<script src=\"js/stats.js\"></script>")
iowl("<script>")
iowl("var count_particles, stats, update;")
iowl("  stats = new Stats;")
iowl("  stats.setMode(0);")
iowl("  stats.domElement.style.position = 'fixed';")
iowl("  stats.domElement.style.left = '10px';")
iowl("  stats.domElement.style.top = '10px';")
iowl("  document.body.appendChild(stats.domElement);")
iowl("  update = function() {")
iowl("    stats.begin();")
iowl("    stats.end();")
iowl("    requestAnimationFrame(update);")
iowl("  };")
iowl("  requestAnimationFrame(update);")
iowl("</script>")
iowl("<div class = \"title\">")
iowl("Source : " .. source .. "<br/>")
iowl("Results for: " .. itemname)
iowl("</div>")
iowl("<table>")
iowl("<tr>")
iowl("<th>Id</th>")
iowl("<th>Title</th>")
iowl("<th>Link</th>")
iowl("</tr>")




local guard = true

while(guard == true)
do
local _beg1,_beg2 = string.find(result, "btih:")
if(_beg1 == nil)
then
guard = false
else

result = string.sub(result, _beg1 + 5)
local _end = string.find(result,"title=")
_end = _end - 3
local item = string.sub(result,1,_end)
UnpackItem(tostring(item))
end


end
iowl("</table>")
iowl("<div class = \"credits\">")
iowl("</br>Made by Ongornbk </br> All Rights Reserved ©")
iowl("</div>")
iowl("</div>")
iowl("</body>")
iowl("</html>")
fs.Close()
end

