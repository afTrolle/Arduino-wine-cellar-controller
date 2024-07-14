package trolle.af.wine.cellar.client

import androidx.compose.foundation.Image
import androidx.compose.foundation.interaction.MutableInteractionSource
import androidx.compose.foundation.interaction.collectIsPressedAsState
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.widthIn
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Lock
import androidx.compose.material.icons.filled.LockOpen
import androidx.compose.material.icons.filled.Thermostat
import androidx.compose.material.icons.filled.WaterDrop
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.CenterAlignedTopAppBar
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Scaffold
import androidx.compose.material3.SnackbarHost
import androidx.compose.material3.SnackbarHostState
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.vector.ImageVector
import androidx.compose.ui.graphics.vector.rememberVectorPainter
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.unit.dp
import trolle.af.wine.cellar.client.theme.AppTheme
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.delay
import kotlinx.coroutines.isActive
import lajlas_wine_cellar.composeapp.generated.resources.PC288238
import lajlas_wine_cellar.composeapp.generated.resources.Res
import org.jetbrains.compose.resources.imageResource

@Composable
internal fun App() = AppTheme {

    var climate: Weather? by remember { mutableStateOf(null) }
    val url = "192.168.1.117"

    LaunchedEffect(Unit) {
        climate = CellarClient.getWeather(url)
    }

    MainScreenLayout(
        climate,
        whileOpen = {
            while (isActive) {
                CellarClient.open(url)
                delay(350)
            }
        },
        whileClose = {
            while (isActive) {
                CellarClient.close(url)
                delay(350)
            }
        }
    )
}


@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun MainScreenLayout(
    climate: Weather? = null,
    whileOpen: suspend CoroutineScope.() -> Unit = {},
    whileClose: suspend CoroutineScope.() -> Unit = {},
    snackbarHostState: SnackbarHostState = remember { SnackbarHostState() },
) = Scaffold(
    modifier = Modifier.fillMaxSize(),
    containerColor = MaterialTheme.colorScheme.surfaceContainer,
    snackbarHost = { SnackbarHost(snackbarHostState) },
    topBar = {
        CenterAlignedTopAppBar(
            title = {
                Text("Lajlas Vinkällare")
            },
        )
    }
) { paddingValues ->
    Box(
        Modifier.fillMaxSize()
    ) {
        val image = imageResource(Res.drawable.PC288238)
        Image(
            bitmap = image,
            contentDescription = null,
            modifier = Modifier.fillMaxSize(),
            contentScale = ContentScale.Crop
        )
        Column(
            Modifier.fillMaxSize(),
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            Column(
                Modifier
                    .widthIn(max = 500.dp)
                    .padding(paddingValues)
                    .padding(16.dp),
                verticalArrangement = Arrangement.spacedBy(16.dp)
            ) {

                ClimateInfo(climate)
                WineControls(
                    whileOpen = whileOpen,
                    whileClose = whileClose,
                )
            }
        }
    }
}

@Composable
fun ClimateInfo(climate: Weather?) = Card(
    colors = CardDefaults.elevatedCardColors()
) {
    Column(
        Modifier.padding(16.dp)
    ) {
        Text(
            "Klimat",
            modifier = Modifier.fillMaxWidth(),
            style = MaterialTheme.typography.headlineMedium
        )
        Row(
            Modifier.fillMaxWidth(),
            horizontalArrangement = Arrangement.spacedBy(16.dp)
        ) {
            ClimateCard(
                Modifier.weight(1f),
                image = Icons.Default.Thermostat,
                text = climate?.temperature?.let { "${(it - 6f).toTwoDecimalString()} °C" }
                    ?: "NaN °C"
            )
            ClimateCard(
                Modifier.weight(1f),
                image = Icons.Default.WaterDrop,
                text = climate?.humidity?.let {
                    "${it.toTwoDecimalString()} %"
                } ?: "NaN %"
            )
        }
    }
}

@Composable
fun ClimateCard(
    modifier: Modifier,
    image: ImageVector,
    text: String,
) = Card(modifier) {
    Column(
        modifier = Modifier.fillMaxWidth().padding(vertical = 16.dp),
        horizontalAlignment = Alignment.CenterHorizontally,
        verticalArrangement = Arrangement.Center
    ) {
        val resource = rememberVectorPainter(image)
        Icon(
            painter = resource,
            contentDescription = null
        )
        Spacer(Modifier.height(4.dp))
        Text(
            text = text,
            style = MaterialTheme.typography.titleMedium
        )
    }
}


@Composable
private fun WineControls(
    modifier: Modifier = Modifier,
    whileOpen: suspend CoroutineScope.() -> Unit = {},
    whileClose: suspend CoroutineScope.() -> Unit = {},
) = Card(
    colors = CardDefaults.elevatedCardColors()
) {
    Column(
        Modifier.padding(16.dp)
    ) {
        Text(
            "Vinluckan",
            modifier = Modifier.fillMaxWidth(),
            style = MaterialTheme.typography.headlineMedium
        )
        Spacer(Modifier.height(8.dp))
        Row(
            modifier = modifier,
            horizontalArrangement = Arrangement.spacedBy(16.dp)
        ) {
            IconCardButton(
                Modifier.weight(1f),
                text = "Öppna",
                image = Icons.Default.LockOpen,
                whilePressed = whileOpen
            )
            IconCardButton(
                Modifier.weight(1f),
                text = "Stäng",
                image = Icons.Default.Lock,
                containerColor = MaterialTheme.colorScheme.secondaryContainer,
                whilePressed = whileClose
            )
        }
    }
}

@Composable
private fun IconCardButton(
    modifier: Modifier = Modifier,
    image: ImageVector = Icons.Default.Lock,
    text: String = "open",
    containerColor: Color = MaterialTheme.colorScheme.tertiaryContainer,
    interactionSource: MutableInteractionSource = remember { MutableInteractionSource() },
    whilePressed: suspend CoroutineScope.() -> Unit = {},
) {
    val pressed by interactionSource.collectIsPressedAsState()
    LaunchedEffect(pressed) {
        if (pressed) {
            whilePressed()
        }
    }

    Card(
        modifier = modifier,
        onClick = { },
        interactionSource = interactionSource,
        colors = CardDefaults.cardColors(containerColor = containerColor)
    ) {
        Column(
            modifier = Modifier.fillMaxWidth().padding(8.dp),
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            val resource = rememberVectorPainter(image)
            Icon(painter = resource, contentDescription = null)
            Text(
                text = text,
                style = MaterialTheme.typography.titleMedium
            )
        }
    }
}

