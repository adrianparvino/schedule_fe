/** @type {import('tailwindcss').Config} */
export default {
  content: [
    './index.html',
    'src/**/*.re'
  ],
  theme: {
    extend: {
      colors: {
        primary: '#59b259',
      },
      gridRowStart: Object.fromEntries(Array.from({length: 49}, (_, i) => [`${i + 1}`, `${i + 1}`])),
      gridRowEnd: Object.fromEntries(Array.from({length: 49}, (_, i) => [`${i + 1}`, `${i + 1}`])),
      gridRowSpan: Object.fromEntries(Array.from({length: 49}, (_, i) => [`${i + 1}`, `${i + 1}`])),
      gridTemplateRows: Object.fromEntries(Array.from({length: 49}, (_, i) => [`cal${i + 1}`, `max-content repeat(${i + 1}, minmax(0, 1fr))`])),
      gridTemplateColumns: {
        'cal': 'max-content repeat(7, minmax(0, 1fr))',
      }
    },
  },
  safelist: [
    { pattern: /row-start-\d+/},
    { pattern: /row-end-\d+/},
    { pattern: /row-span-\d+/},
    { pattern: /grid-rows-cal\d+/},
    { pattern: /col-start-\d+/},
  ],
  plugins: [],
}

let morse_digits = [
  "-----",
  ".----",
  "..---",
  "...--",
  "....-",
  ".....",
  "-....",
  "--...",
  "---..",
  "----.",
]

function morse(n) {
  let output = [];
  while (n > 0) {
    output.push(morse_digits[n % 10]);
    n = Math.floor(n / 10);
  }

  output.reverse();
  return output.join("");
}