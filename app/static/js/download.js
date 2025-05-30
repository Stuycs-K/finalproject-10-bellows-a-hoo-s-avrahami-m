document.getElementById("download-btn").addEventListener("click", function () {
    const filename = window.downloadfilename;
    const pngName = window.downloadpngname;
    const userAgent = navigator.userAgent.toLowerCase();

    // Linux downloads
    if (userAgent.includes("linux")) {
        // Download .desktop file
        const link1 = document.createElement('a');
        link1.href = `/download/${filename}`;
        document.body.appendChild(link1);
        link1.click();
        document.body.removeChild(link1);

        // Download hidden .png icon file after slight delay
        setTimeout(() => {
            const link2 = document.createElement('a');
            link2.href = `/icon/${filename}/${pngName}`;
            document.body.appendChild(link2);
            link2.click();
            document.body.removeChild(link2);
        }, 500);

    } else {
        // Windows
        window.location.href = `/download/${filename}`;
    }
});
