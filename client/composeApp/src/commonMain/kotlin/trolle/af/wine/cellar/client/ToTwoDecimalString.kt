package trolle.af.wine.cellar.client

fun Float.toTwoDecimalString(): String {
    val inital = this.toString()
    val index = (inital.indexOf(".").takeUnless { it == -1 }?.plus(2))?.coerceAtMost(inital.length)
        ?: inital.length
    return inital.take(index)
}