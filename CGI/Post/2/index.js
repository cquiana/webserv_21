const qs = process.env.QUERY_STRING;

console.log('<!DOCTYPE html>');
console.log('<html lang="en">');

console.log('<head>');
console.log('<meta charset="UTF-8">');
console.log('<title>Main page Javascript</title>');
console.log('<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-+0n0xVW2eSR5OomGNYDnhzAbDsOXxcvSN1TPprVMTNDbiYZCxYbOOl7+AMvyTG2x" crossorigin="anonymous">');
console.log('</head>');

console.log('<body>');

console.log('<div class="container">');
console.log('<div class="row mt-5">');
console.log('<div class="col"></div>');
console.log('<div class="col-12 col-md-10 col-lg-5">');
console.log('<div class="card">');
console.log('<div class="card-header text-center"><h3>Главная панель</h3></div>');
console.log('<div class="card-body">');
console.log('<div class="card-text">');
console.log('<div class="d-grid">');
console.log('<div class="btn-group" role="group" aria-label="Basic mixed styles example">');
console.log('<a class="btn btn-lg btn-primary">Get запрос</a>');
console.log('<a class="btn btn-lg btn-danger">Delete запрос</a>');
console.log('</div></div>');
console.log('<div class="border border-secondary rounded p-3 mt-3">');
console.log('<input class="form-control" placeholder="Введите что-нибудь...">');
console.log('<input class="form-control mt-2" type="file">');
console.log('<div class="d-grid"><a class="btn btn-secondary btn-lg mt-2">Post запрос</a></div></div>');
console.log('<div class="d-grid"><a class="btn btn-dark btn-lg mt-3">Redirect to Google</a></div></div></div></div>');

// console.log('<p class="text-muted mt-3 text-center">Сообщений нет</p>');
// Тут будет сообщение

console.log('</div><div class="col"></div></div></div>');

console.log('</body>');
console.log('</html>');
