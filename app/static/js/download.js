document.getElementById("download-btn").addEventListener("click", function () {
    const filename = "{{ download_filename }}";
    const userAgent = navigator.userAgent.toLowerCase();

    if (userAgent.includes("linux")) {
        // Linux (download .desktop and .png with one click)
        const link1 = document.createElement('a');
        link1.href = `/download/${filename}`;
        link1.download = '';
        document.body.appendChild(link1);
        link1.click();
        document.body.removeChild(link1);

        setTimeout(() => {
            const link2 = document.createElement('a');
            link2.href = `/icon/${filename}`;
            link2.download = '';
            document.body.appendChild(link2);
            link2.click();
            document.body.removeChild(link2);
        }, 500);

    } else {
        // Windows
        window.location.href = `/download/${filename}`;
    }
});
