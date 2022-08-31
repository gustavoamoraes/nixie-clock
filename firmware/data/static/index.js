// Simple example, see optional options for more configuration.
const pickr = Pickr.create({
    el: '.pickr-container',
    theme: 'monolith', // or 'monolith', or 'nano'
    adjustableNumbers: true,
    autoReposition: true,
    components: {

        // Main components
        preview: true,
        opacity: false,
        hue: true,

        // Input / output Options
        interaction: {
            hex: true,
            rgba: true,
            hsla: false,
            hsva: false,
            cmyk: false,
            input: true,
            clear: false,
            save: true
        }
    },
});

var nixieSlider = $(".nixieSlider");
nixieSlider.on('input', (e) => 
{
    $(".nixieSliderLabel").text(`(${nixieSlider.val()}%)`);
});

//Make post request with configs in json
function onApply ()
{
    let bg_color = pickr.getColor().toRGBA();

    var config_data = 
    {   
        "background_color" : [Math.floor(bg_color[0]), Math.floor(bg_color[1]), Math.floor(bg_color[2])],
        "digits_brigthness" : nixieSlider.val()/100
    }

    fetch("/config", {
      method: "POST",
      headers: {'Content-Type': 'application/json'}, 
      body: JSON.stringify(config_data)
    }).then(res => {
      console.log("Request complete! response:", res);
    });
}