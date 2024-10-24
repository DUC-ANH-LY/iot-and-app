var check = true;
function changeImageButton() {
    var imgButton = document.getElementById("imgButton");
    var ledStatus = document.getElementById("statusLed")
    if (imgButton.src.endsWith("off-removebg-preview.png")) {
        ledStatus.src = "images/fanoff-removebg-preview.png";
        imgButton.src = "images/on-removebg-preview.png";
        addNewRow("đèn 1", "ON");
    } else {
        ledStatus.src = "images/fanon-removebg-preview.png"
        imgButton.src = "images/off-removebg-preview.png";
        addNewRow("đèn 1", "OFF");
    }}
    function changeImageButton1() {
        var imgButton1 = document.getElementById("imgButton1");
        var ledStatus1 = document.getElementById("statusLed1")
        if (imgButton1.src.endsWith("off-removebg-preview.png")) {
            ledStatus1.src = "images/lon-removebg-preview.png";
            imgButton1.src = "images/on-removebg-preview.png";
            addNewRow("đèn 2", "ON");
        } else {
            ledStatus1.src = "images/loff-removebg-preview.png"
            imgButton1.src = "images/off-removebg-preview.png";
            addNewRow("đèn 2", "OFF");
        }
}

