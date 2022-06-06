//Slideshow - Serviços

var slides = document.querySelectorAll('.slides1 .slide');
var currentSlide = 0;
var slideInterval = setInterval(nextSlide,5000);


var slides2 = document.querySelectorAll('.slides2 .slide');
var currentSlide2 = 0;
var slideInterval2 = setInterval(nextSlide2,5000);

function nextSlide() {
    slides[currentSlide].className = 'slide';
    currentSlide = (currentSlide+1)%slides.length;
    slides[currentSlide].className = 'slide mostrar';
}

function nextSlide2() {
    slides2[currentSlide2].className = 'slide';
    currentSlide2 = (currentSlide2+1)%slides2.length;
    slides2[currentSlide2].className = 'slide mostrar';
}





