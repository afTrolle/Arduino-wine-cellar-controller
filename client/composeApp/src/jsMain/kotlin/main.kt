import androidx.compose.ui.ExperimentalComposeUiApi
import androidx.compose.ui.window.CanvasBasedWindow
import org.jetbrains.skiko.wasm.onWasmReady
import trolle.af.wine.cellar.client.App

@OptIn(ExperimentalComposeUiApi::class)
fun main() {
    onWasmReady {
        CanvasBasedWindow("Lajlas Vinkällare", "ComposeTarget") {
            App()
        }
    }
}