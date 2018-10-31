function decorator(options) {
  // options are passed with the decorator method
  return function(descriptor) {
    // alter the descriptor to change its properties and return it
    descriptor.finisher = function(klass) {
      // now you get the class it was defined on at the end
    }
    return descriptor;
  }
}

function deprecated(descriptor) {
  let fn = descriptor.descriptor.value;
  descriptor.descriptor.value = function() {
    console.log('this function is deprecated!');
    return fn.apply(this, arguments);
  }
  return descriptor;
}

function readonly(descriptor) {
  descriptor.descriptor.writable = false;
  return descriptor;
}

function property(descriptor) {
  descriptor.finisher = function(klass) {
    klass.properties = klass.properties || [];
    klass.properties.push(descriptor.key);
  };
  return descriptor;
}

class DecoratorExample {
  @decorator({ foo: 'bar' })
  decorated() {
    return 'foo'
  }

  @deprecated
  oldFunc(val) {
    return 'oldFunc: ' + val;
  }

  @readonly
  x = 4;

  @property
  someProp = 5;

  static listProperties() {
    return this.properties || [];
  }
}

let ex = new DecoratorExample();

// the deprecated function should warn when you call it
ex.oldFunc('foo');

// the `x` prop is readonly
ex.x = 5;
console.log(ex.x);

// class reflection: list all the properties with the @property decorator
let properties = DecoratorExample.listProperties();
console.log(properties);
