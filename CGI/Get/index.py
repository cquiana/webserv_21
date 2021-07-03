print("Content-type:text/html\r\n\r\n")

print('<!DOCTYPE html>')
print('<html lang="en">')

print('<head>')
print('<meta charset="UTF-8">')
print('<title>Main page Python</title>')
print('<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-+0n0xVW2eSR5OomGNYDnhzAbDsOXxcvSN1TPprVMTNDbiYZCxYbOOl7+AMvyTG2x" crossorigin="anonymous">')
print('</head>')

print('<body>')

print('<div class="container">')
print('<div class="row mt-5">')
print('<div class="col"></div>')
print('<div class="col-12 col-md-10 col-lg-5">')
print('<div class="card">')
print('<div class="card-header text-center"><h3>Главная панель</h3></div>')
print('<div class="card-body">')
print('<div class="card-text">')
print('<div class="d-grid">')
print('<div class="btn-group" role="group" aria-label="Basic mixed styles example">')
print('<button type="button" class="btn btn-lg btn-primary">Get запрос</button>')
print('<button type="button" class="btn btn-lg btn-danger">Delete запрос</button>')
print('</div></div>')
print('<div class="border border-secondary rounded p-3 mt-3">')
print('<input class="form-control" placeholder="Введите что-нибудь...">')
print('<input class="form-control mt-2" type="file">')
print('<div class="d-grid"><a class="btn btn-secondary btn-lg mt-2">Post запрос</a></div></div>')
print('<div class="d-grid"><button class="btn btn-dark btn-lg mt-3" type="button">Redirect to Google</button></div></div></div></div>')

print('<p class="text-muted mt-3 text-center">Сообщений нет</p>')

print('</div><div class="col"></div></div></div>')

print('</body>')
print('</html>')
