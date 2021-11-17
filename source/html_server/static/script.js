const pickr = Pickr.create({
    el: '.pickr',
    theme: 'classic',

    components:
    {
        preview: true,
        opacity: false,
        hue: true,

        interaction: {
            hex: true,
            rgba: true,
            hsla: true,
            hsva: false,
            cmyk: false,
            input: true,
            clear: true,
            save: true
        }
    }
});

digits_range = $('#b_control_0'); digits_range_label = $('#b_control_0_label')
bg_range = $('#b_control_1'); bg_range_label = $('#b_control_1_label')
// ring_range = $('#b_control_2'); ring_range_label = $('#b_control_2_label')

last_bg_color = [0,0,0,0]

// change_label_range (ring_range, ring_range_label);
change_label_range (digits_range, digits_range_label);

function change_label_range (range, label)
{
    range.on('input', function() 
    {
        label.text(`(${range.val()}%)`);
    });
}

bg_range.on('input', function () 
{
    val = bg_range.val();
    max = last_bg_color[2]
    scale = Math.round(digits_range.val()/100)

    pickr.setColor(`hsl (${last_bg_color[0]}, ${last_bg_color[1]}, ${scale * max})`, true);
    pickr.applyColor(true);

    bg_range_label.text(`(${val}%)`);
});

pickr.on('save', (color) => {
    last_bg_color = color.toHSLA()
    bg_range.val(100).trigger("input")
});

digits_range.trigger('input')
bg_range.trigger('input')
// ring_range.trigger('input')

var submit_button = $('#submit_button')
submit_button.on('click', on_submit);

function on_submit ()
{   
    let rgb_color = pickr.getColor().toRGBA();
    let brigthness_scale = Math.round(digits_range.val()/100);

    const data = {
            "config": 
                    {
                        "digits_brightness": brigthness_scale,
                        "background_color": `${rgb_color[0]}, ${rgb_color[1]}, ${rgb_color[2]}`,
                    }
                }

    fetch("../apí/config", 
    {
        method: "POST",
        headers: 
        {
          "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
    })
}