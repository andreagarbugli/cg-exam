module.exports = {
    title: "Progetto Fondamenti di Computer Grafica",
    description: "Just playing around",
    // base: "/docs/",
    base: "/cg-exam/",
    dest: "docs",
    themeConfig: {
        sidebar: ["/", "engine-core", "graphics", "game", "utils"],
        // sidebar: [
        //     {
        //         title: "Introduzione",
        //         collapsable: false,
        //         children: ["/"]
        //     }
        // ],
        sidebarDepth: 2
    },
    markdown: {
        extendMarkdown: md => {
            // use more markdown-it plugins!
            md.use(require("markdown-it-katex"));
        }
    }
};
