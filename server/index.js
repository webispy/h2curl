require('console-stamp')(console, '[HH:MM:ss.l]');

const http2 = require('http2');
const fs = require('fs')
const url = require('url');

const server = http2.createSecureServer({
  key: fs.readFileSync('./server.key'),
  cert: fs.readFileSync('./server.crt')
});

const boundary = 'example-boundary-1234';

function pathLongPolling(stream) {
  /* send headers */
  stream.respond({
    ':status': 200,
    'Content-Type': `multipart/related; boundary=${boundary}`,
  });

  /* send dummy data */
  stream.write(`--${boundary}\r`);

  /* stream end after 10 secs */
  setTimeout(() => {
    console.log('finish the get stream');
    stream.end('GET-FINISHED')
  }, 10000);
}

function pathPost(stream) {
  stream.respond({':status': 200});

  stream.on('close', () => {
    console.log('event stream closed');
  });
  stream.on('data', chunk => {
    console.log(`post data: ${chunk}`);
  });
  stream.on('error', code => {
    console.log('event stream error:', code);
  });

  /* stream end after 5 secs */
  setTimeout(() => {
    console.log('finish the post stream');
    stream.end('POST-FINISHED');
  }, 5000)
}

server.on('stream', (stream, headers) => {
  console.log('Stream:', stream.id, ', Received new request:', headers);
  const reqPath = url.parse(headers[':path']);

  switch (reqPath.pathname) {
    case '/longPolling':
      pathLongPolling(stream, headers);
      break;
    case '/post':
      pathPost(stream, headers);
      break;
    default:
      console.log('invalid request:', reqPath.pathname);
      break;
  }
});

server.on('session', session => {
  console.log('---- new session ----', session.socket.remoteAddress);
})

server.listen(8081);
