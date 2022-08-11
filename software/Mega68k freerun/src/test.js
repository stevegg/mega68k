const VPA_PIN = 4;
const BERR_PIN = 1;
const BGACK_PIN = 0;

var result = 255;

function printBits(value, size) {
    var buffer = [];
    for ( var i = 0; i < size; i ++ ) {
       buffer[i] = (value & (1<<(size-i))) ? '1' : '0';
    }
    return buffer.join("");
  }
  

result &= ~(
    (1<<VPA_PIN) |
    (1<<BERR_PIN) |
    (1<<BGACK_PIN)
);
console.log(result.toString(2));

result = 0;
result |= ((1<<BERR_PIN) | (1<<VPA_PIN) | (1<<BGACK_PIN));
console.log(result.toString(2));

var hi  = 0b01010000;
var low = 0b00000001;

result = (hi<<8) | low;
console.log(result.toString(16));
result = ((hi << 8) | low) << 1;
console.log(result.toString(16));

// Test getting hi and low byte values from a word

var data = 0xA0B2;
var low = data & 0xFF;
var hi = data >> 8;
console.log("Hi Byte: ", hi.toString(16));
console.log("Lo Byte: ", low.toString(16));

var value = 0b01011011010111101010010;
value = value << 1;
console.log("Value: ", value.toString(2));
console.log("Value: ", value.toString(16));

var xAddress = 0xAD & ~(1<<7); // 0x2D;
var hAddress = 0xAF;
var lAddress = 0x52;
var address = ((xAddress<<16) | (hAddress<<8) | lAddress) << 1;

console.log("Address: ", address.toString(16));
console.log("Address: ", printBits(address, 24));

xAddress = 0x04;
hAddress = 0xAF;
lAddress = 0x6D;
address = ((xAddress<<16) | (hAddress<<8) | lAddress) << 1;
console.log("Address: ", address.toString(16));
console.log("Address: ", printBits(address, 24));
