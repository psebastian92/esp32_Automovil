// Elemento que muestra el estado de conexión
const status = document.getElementById('status');

// Crear conexión WebSocket al ESP32 (ajustar IP si es necesario)
const socket = new WebSocket('ws://192.168.0.110:81');

// Evento: conexión establecida
socket.onopen = () => {
  status.textContent = '✅ Conectado';
  status.style.backgroundColor = '#b6fcb6'; // verde suave
};

// Evento: conexión cerrada
socket.onclose = () => {
  status.textContent = '❌ Desconectado';
  status.style.backgroundColor = '#fcb6b6'; // rojo suave
};

// Mapeo de teclas con sus elementos visuales para efecto de pulsación
const keyMap = {
  w: document.getElementById('key-w'),
  a: document.getElementById('key-a'),
  s: document.getElementById('key-s'),
  d: document.getElementById('key-d'),
  '1': document.getElementById('key-1'),
  '2': document.getElementById('key-2')
};

// Conjunto de teclas presionadas actualmente
const pressedKeys = new Set();

// Referencia al SVG del auto para animar su movimiento
const car = document.getElementById('car');

// Guardar último comando enviado para evitar repetidos
let lastCommand = '';
let stopInterval = null;
/**
 * Actualiza el movimiento y estado del auto según teclas presionadas.
 */
function updateMovement() {
  // Si la conexión no está abierta, no hacer nada
  if (socket.readyState !== WebSocket.OPEN) return;

  // Obtener todas las teclas presionadas, ordenadas para identificar combinaciones
  const keys = Array.from(pressedKeys).sort().join('');
  let command = '';
  let transform = '';

  // Determinar comando y transformación visual según teclas presionadas
  switch (keys) {
    case 'w':
      command = 'w';                    // Adelante
      transform = 'translateY(-10px)'; // Mover arriba (eje Y negativo)
      break;
    case 'a':
      command = 'a';                   // Izquierda
      transform = 'translateX(-10px)'; // Mover a la izquierda (eje X negativo)
      break;
    case 's':
      command = 's';                   // Atrás
      transform = 'translateY(10px)'; // Mover abajo (eje Y positivo)
      break;
    case 'd':
      command = 'd';                   // Derecha
      transform = 'translateX(10px)'; // Mover a la derecha (eje X positivo)
      break;
    case 'aw':
      command = 'q';                          // Adelante + Izquierda (diagonal)
      transform = 'translate(-7px, -7px) rotate(-10deg)';
      break;
    case 'dw':
      command = 'e';                          // Adelante + Derecha (diagonal)
      transform = 'translate(7px, -7px) rotate(10deg)';
      break;
    case 'as':
      command = 'z';                          // Atrás + Izquierda (diagonal)
      transform = 'translate(-7px, 7px) rotate(-10deg)';
      break;
    case 'ds':
      command = 'c';                          // Atrás + Derecha (diagonal)
      transform = 'translate(7px, 7px) rotate(10deg)';
      break;
    case '1':
      command = '1';                   // Giro antihorario
      transform = 'rotate(-15deg)';
      break;
    case '2':
      command = '2';                   // Giro horario
      transform = 'rotate(15deg)';
      break;
    default:
      command = 'stop';                // Sin teclas o combinación no reconocida
      transform = 'translate(0, 0) rotate(0)';
      break;
  }

  // Enviar comando solo si cambió respecto al anterior
  if (command !== lastCommand) {
    socket.send(command);
    lastCommand = command;
  }

    // Si no hay teclas presionadas, arrancar intervalo para enviar stop cada 100ms
  if (command === 'stop' && stopInterval === null) {
    stopInterval = setInterval(() => {
      if (socket.readyState === WebSocket.OPEN) {
        console.log('Enviando stop');  // Para verificar que se envía cada 100ms
        socket.send('stop');
      }
    }, 100);
  }

  // Si hay alguna tecla y está activo el intervalo, lo paro
  if (command !== 'stop' && stopInterval !== null) {
    clearInterval(stopInterval);
    stopInterval = null;
  }

  // Aplicar transformación visual al auto
  car.style.transform = transform;

  // Actualizar clases visuales de las teclas según si están presionadas
  for (let k in keyMap) {
    if (pressedKeys.has(k)) {
      keyMap[k].classList.add('pressed');
    } else {
      keyMap[k].classList.remove('pressed');
    }
  }
}

// Eventos para detectar teclas presionadas
window.addEventListener('keydown', e => {
  const key = e.key.toLowerCase();

  // Si es una tecla que controlamos y no estaba presionada, agregarla y actualizar
  if (keyMap[key] && !pressedKeys.has(key)) {
    pressedKeys.add(key);
    updateMovement();
  }
});

// Evento para detectar tecla liberada
window.addEventListener('keyup', e => {
  const key = e.key.toLowerCase();

  // Si estaba presionada, eliminarla y actualizar
  if (keyMap[key] && pressedKeys.has(key)) {
    pressedKeys.delete(key);
    updateMovement();
  }
});

// Eventos para hacer que al clickear en los botones en pantalla se comporte igual que con teclado
for (let key in keyMap) {
  const elem = keyMap[key];
  elem.addEventListener('mousedown', () => {
    if (!pressedKeys.has(key)) {
      pressedKeys.add(key);
      updateMovement();
    }
  });
  elem.addEventListener('mouseup', () => {
    if (pressedKeys.has(key)) {
      pressedKeys.delete(key);
      updateMovement();
    }
  });
  // Para tocar y soltar en móviles
  elem.addEventListener('touchstart', e => {
    e.preventDefault(); // evitar evento mouse
    if (!pressedKeys.has(key)) {
      pressedKeys.add(key);
      updateMovement();
    }
  });
  elem.addEventListener('touchend', e => {
    e.preventDefault();
    if (pressedKeys.has(key)) {
      pressedKeys.delete(key);
      updateMovement();
    }
  });
}
