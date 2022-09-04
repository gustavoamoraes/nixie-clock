const pickr = Pickr.create(
{
    el: '.pickr-container',
    theme: 'monolith', // or 'monolith', or 'nano'

    components: {

        // Main components
        preview: true,
        opacity: false,
        hue: true,

        // Input / output Options
        interaction: {
            hex: true,
            rgba: true,
            input: true,
            save: true
        }
    },
});

//Get elements
var nixieSlider = $("#nixieSlider");
var ringSlider = $("#ringSlider");
var idleCheck = $("#idleCheckBox");
var idleTimeInput = $("#idleTimeInput");
var nixieSliderLabel = $("#nixieSliderLabel");
var ringSliderLabel = $("#ringSliderLabel");

bindLabelToSlider(nixieSlider, nixieSliderLabel)
bindLabelToSlider(ringSlider, ringSliderLabel)

pickr.on('init', instance => 
{
    getSavedConfig ();
});

function getSavedConfig ()
{
    fetch('/config')
    .then((response)=>response.json())
    .then((responseJson)=> { applyConfig(responseJson);});
}

function bindLabelToSlider (slider, label)
{
    slider.on('input', (e) => 
    {
        label.text(`(${slider.val()}%)`);
    });
}

function applyConfig (config)
{   
    var bgColor = `rgb(${config["background_color"][0]}, ${config["background_color"][1]}, ${config["background_color"][2]})`;
    pickr.setColor(bgColor, false);
    nixieSlider.val(config["digits_brightness"] * 100).trigger("input");
    ringSlider.val(config["led_ring_brightness"] * 100).trigger("input");
    idleTimeInput.val(config["time_to_idle"]);
    idleCheck.prop('checked', config["to_idle"]);
}

//Make post request with configs in json
function onApply ()
{
    let bg_color = pickr.getColor().toRGBA();

    var config_data = 
    {   
        "background_color" : [Math.floor(bg_color[0]), Math.floor(bg_color[1]), Math.floor(bg_color[2])],
        "digits_brightness" : nixieSlider.val()/100,
        "led_ring_brightness" : ringSlider.val()/100,
        "to_idle" : idleCheck.is(":checked"),
        "time_to_idle" : parseInt(idleTimeInput.val())
    }

    fetch("/config", {
      method: "POST",
      headers: {'Content-Type': 'application/json'}, 
      body: JSON.stringify(config_data)
    }).then(res => {
      console.log("Request complete! response:", res);
    });
}
